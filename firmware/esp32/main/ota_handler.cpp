/*******************************************************************************
  ; -----------------------------------------------------------------------
   gbaHD-ESP32 for zwenergys gbaHD

   MIT License

   Copyright (c) 2021 Alexander Kreutz

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

 *******************************************************************************/

#include "ota_handler.h"

#include <LittleFS.h>
#include <WiFi.h>
#include <Update.h>
#include <cJSON.h>
#include <uni_bluetooth.h>

#include "bitstream_handler.h"
#include "log_handler.h"
#include "web_handler.h"
#include "mega_handler.h"

OTA_Handler_Class OTA_Handler;


uint32_t startTime = 0U;


void OTA_Handler_Class::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{

    if (type == WS_EVT_CONNECT)
    {
        Log_Handler.println("Websocket client connection received");
        if (OTA_Handler.ota_state == OTA_STATE_NONE)
        {
            OTA_Handler.update_latest_BS();
        }
        OTA_Handler.ws_client = client;
    } 
    else if (type == WS_EVT_DISCONNECT)
    {
        OTA_Handler.ws_client = nullptr;
        Log_Handler.println("Websocket client connection finished");
    }
}


bool OTA_Handler_Class::get_update_available(void)
{
    return true;
}


void OTA_Handler_Class::refresh_update_info(Update_Info& info, const String& url)
{
    HTTPClient client;

    if ((client.begin(url)
            && (HTTP_CODE_OK == client.GET())))
    {
        String jsonString = client.getString();

        cJSON* release_json = cJSON_Parse(jsonString.c_str());
        if (cJSON_HasObjectItem(release_json, "body")
                && cJSON_HasObjectItem(release_json, "name")
                && cJSON_HasObjectItem(release_json, "assets"))
        {
            uint8_t update_idx = 0U;
            info.changelog = cJSON_GetObjectItem(release_json, "body")->valuestring;
            info.changelog.replace("\n", "<br>");
            info.version = cJSON_GetObjectItem(release_json, "name")->valuestring;

            cJSON* assets = cJSON_GetObjectItem(release_json, "assets");

            for (uint8_t i = 0U; i < cJSON_GetArraySize(assets); i++)
            {
                cJSON* asset = cJSON_GetArrayItem(assets, i);

                if (cJSON_HasObjectItem(asset, "browser_download_url"))
                {
                    String url = String(cJSON_GetObjectItem(asset, "browser_download_url")->valuestring);
                    //url.toLowerCase();

                    if (url.indexOf("gbahd_esp32") != -1)
                    {
                        Log_Handler.println("ESP URL: " + url);
                        info.urls[update_idx].ota_part = OTA_ESP;
                        info.urls[update_idx].url = url;
                        update_idx++;
                    }
                    else if (url.indexOf("gbahd_spiffs") != -1)
                    {
                        Log_Handler.println("SPIFFS URL: " + url);
                        info.urls[update_idx].ota_part = OTA_SPIFFS;
                        info.urls[update_idx].url = url;
                        update_idx++;
                    }
                    else if (url.indexOf("720") != -1)
                    {
                        Log_Handler.println("720P URL: " + url);
                        info.urls[update_idx].ota_part = OTA_BS_720;
                        info.urls[update_idx].url = url;
                        update_idx++;
                    }
                    else if (url.indexOf("1080") != -1)
                    {
                        Log_Handler.println("1080P URL: " + url);
                        info.urls[update_idx].ota_part = OTA_BS_1080;
                        info.urls[update_idx].url = url;
                        update_idx++;
                    }

                    //info.changelog += url;
                }
            }
            for (;update_idx < (sizeof(info.urls)/sizeof(info.urls[0])); update_idx++)
            {
                info.urls[update_idx].ota_part = OTA_NONE;
            }
        }

        client.end();
    }
    else
    {
        Log_Handler.println("Failed to get url ");
    }
}

void OTA_Handler_Class::get_update_info(Update_Info& info)
{
    if (!bs_update_info.checked)
    {
        refresh_update_info(bs_update_info, OTA_BS_RELEASE_URL);
        bs_update_info.checked = true;
    }

    info = bs_update_info;
}

void OTA_Handler_Class::write_to_current_output(uint8_t* buffer, size_t size, bool final)
{
    switch (current_part)
    {
    case OTA_ESP:
    case OTA_SPIFFS:
        if (Update.isRunning())
        {
            if (Update.write(buffer, size) != size)
            {
                Update.printError(Serial);
            }
            else if (final)
            {
                Update.end(true);
            }
        }
        break;
    case OTA_ATMEGA:
    case OTA_BS_720:
    case OTA_BS_1080:
        if (outputFile)
        {
            size_t bytes_written = outputFile.write(buffer, size);
            outputFile.flush();
            if (bytes_written != size)
            {
                Log_Handler.println(outputFile.getWriteError());
                Log_Handler.println("Error writing Update to file. Requested " + String(size) + " wrote " + String(bytes_written));
            }
            else if (final)
            {
                outputFile.close();
            }
        }
        break;
    default:
        break;    
    }
}

bool OTA_Handler_Class::initialize_ota(OTA_Part_Mapping& part)
{
    bool retval = false;

    Log_Handler.println("Initializing OTA for part " + String(part.ota_part));

    if (WiFi.isConnected())
    {
        if (!stream)
        {
            Log_Handler.println("Trying to get url " + part.url);

            http_client.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
            http_client.setRedirectLimit(10);
            http_client.setReuse(true);
            http_client.setTimeout(10000);

            http_client.begin(part.url);
            int returncode = http_client.GET();

            if ( (HTTP_CODE_OK == returncode))
            {
                stream = http_client.getStreamPtr();
                        
                totalSize = http_client.getSize();
                remainingSize = totalSize;

                switch (part.ota_part)
                {
                    case OTA_ESP:
                        retval = Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH);
                        break;
                    case OTA_SPIFFS:
                        retval = Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS);
                        break;
                    case OTA_BS_720:
                        outputFile = LittleFS.open(BITSTREAM_720P_PATH, "w");
                        retval = outputFile.available();
                        break;
                    case OTA_BS_1080:
                        outputFile = LittleFS.open(BITSTREAM_1080P_PATH, "w");
                        retval = outputFile.available();
                        break;
                    case OTA_ATMEGA:
                        outputFile = LittleFS.open(ATMEGA_SPIFFS_PATH, "w");
                        retval = outputFile.available();
                        break;
                    default:
                        break;
                }
            }
            else
            {
                Log_Handler.println("Failed to get url " + part.url + " Error: " + returncode);
                retval = false;
                ota_state = OTA_STATE_NONE;
            }
        }
    }
    return retval;
}

void OTA_Handler_Class::update(void)
{
    if ((ota_state == OTA_STATE_QUEUED) && (ota_queue_idx >= 0) && (ota_queue_length > ota_queue_idx))
    {
        Log_Handler.println("Queue IDX " + String(ota_queue_idx) + " is " + String(current_update->urls[ota_queue_idx].ota_part));

        if (current_update->urls[ota_queue_idx].ota_part != OTA_NONE)
        {
            current_part = current_update->urls[ota_queue_idx].ota_part;
            current_part_mapping = &current_update->urls[ota_queue_idx];
            if (initialize_ota(*current_part_mapping))
            {
                ota_state = OTA_STATE_RUNNING;
                Log_Handler.println("Getting " + current_part_mapping->url);
            }
        }
    }

    if ((OTA_STATE_RUNNING == ota_state) && WiFi.isConnected())
    {
        uint8_t buffer[1024] = { 0 };

        // read all data from server
        while (http_client.connected() 
                && (remainingSize > 0 || remainingSize == -1) 
                && (stream->available()))
        {
            int readCount = stream->read(buffer, sizeof(buffer)/sizeof(buffer[0]));
            if (readCount > 0)
            {
                if (remainingSize > 0)
                {
                    remainingSize -= readCount;
                }
                write_to_current_output(buffer, readCount, (remainingSize == 0));
            }
            
            Log_Handler.println("Read\t" + String(remainingSize) + "\t/ " + String(totalSize));            
        }
        Log_Handler.printf("State %d, Wifi: %d, http_client: %d Stream: %d\n", ota_state, WiFi.isConnected(), http_client.connected(), stream->available());

        if (remainingSize == 0)
        {
            http_client.setReuse(true);
            stream = nullptr;
            http_client.end();
            
            current_part = OTA_NONE;
            
            if (++ota_queue_idx >= ota_queue_length)
            {
                ota_state = OTA_STATE_COMPLETED;
                ota_queue_length = 0U;
                ota_queue_idx = -1;
            }
            else
            {
                ota_state = OTA_STATE_QUEUED;
                remainingSize = 0U;
                totalSize = 0U;
            }
        }
    }
}

void OTA_Handler_Class::full_update(void)
{
    current_update = &release_update_info;
    uni_bluetooth_enable_new_connections_safe(false);
    ota_queue_length = 2U;
    ota_queue_idx = 0U;
    ota_state = OTA_STATE_QUEUED;
}

void OTA_Handler_Class::update_latest_BS(void)
{
    current_update = &bs_update_info;
    uni_bluetooth_enable_new_connections_safe(false);
    ota_queue_length = 2U;
    ota_queue_idx = 0U;
    ota_state = OTA_STATE_QUEUED;
    Log_Handler.println("Scheduling BS Updates.");
}

void OTA_Handler_Class::run(void)
{
    if (WiFi.isConnected())
    {
        if (!bs_update_info.checked)
        {
            refresh_update_info(bs_update_info, OTA_BS_RELEASE_URL);
            bs_update_info.checked = true;
        }
        if (!release_update_info.checked)
        {
            refresh_update_info(release_update_info, OTA_PROD_RELEASE_URL);
            release_update_info.checked = true;
        }
    }

    update();

    if (ws_client)
    {
        switch (ota_state)
        {
        case OTA_STATE_QUEUED:
        {
            float complete_progress = (static_cast<float>(ota_queue_idx)) / static_cast<float>(ota_queue_length);
            String percent = String(static_cast<int>(100.0 * complete_progress));
            ws_client->text(percent);
            break;
        }
        case OTA_STATE_RUNNING:
        {
            float current_progress = static_cast<float>(totalSize-remainingSize) / static_cast<float>(totalSize);
            float complete_progress = (static_cast<float>(ota_queue_idx) + current_progress) / static_cast<float>(ota_queue_length);
            String percent = String(static_cast<int>(100.0 * complete_progress));
            ws_client->text(percent);
            break;
        }
        case OTA_STATE_COMPLETED:
            ws_client->text("100");
            break;
        case OTA_STATE_NONE:
            break;
        default:
            break;
        }
    }

}

void OTA_Handler_Class::init(void)
{
    ws = new AsyncWebSocket("/ota");
    ws->onEvent(onWsEvent);    
    Web_Handler.addWebSocket(ws);
    ws_client = nullptr;    
    
}

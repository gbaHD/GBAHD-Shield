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
#include <Esp.h>
#include <base64.h>
#include <SD_MMC.h>


#include "bitstream_handler.h"
#include "log_handler.h"
#include "web_handler.h"
#include "mega_handler.h"
#include "preferences_handler.h"

OTA_Handler_Class OTA_Handler;

uint32_t startTime = 0U;

void OTA_Handler_Class::initialize_client(HTTPClient& client)
{
    
    client.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    client.setRedirectLimit(10);
    client.setReuse(true);

    {
        String token = "";
        Preferences_Handler.getOTAToken(token);

        if (token != "")
        {
            client.setAuthorizationType("token");
            client.setAuthorization(token.c_str());
        }
    }
}


void OTA_Handler_Class::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{

    if (type == WS_EVT_CONNECT)
    {
        Log_Handler.println("Websocket client connection received");
        // if (OTA_Handler.ota_state == OTA_STATE_NONE)
        // {
        //     OTA_Handler.update_latest_BS();
        // }
        OTA_Handler.ws_client = client;
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        OTA_Handler.ws_client = nullptr;
        Log_Handler.println("Websocket client connection finished");
    }
    else if (type == WS_EVT_DATA)
    {
        if (String(reinterpret_cast<char*>(data), len) == "bitstream")
        {
            if (OTA_Handler.ota_state == OTA_STATE_NONE)
            {
                OTA_Handler.update_latest_BS();
            }
            OTA_Handler.ws_client = client;
        }
        else if (String(reinterpret_cast<char*>(data), len) == "esp")
        {
            if (OTA_Handler.ota_state == OTA_STATE_NONE)
            {
                OTA_Handler.full_update();
            }
            OTA_Handler.ws_client = client;
        }
    }
}


bool OTA_Handler_Class::get_update_available(void)
{
    return true;
}


void OTA_Handler_Class::refresh_update_info(Update_Info& info, const String* url)
{
    HTTPClient client;

    initialize_client(client);

    if (client.begin(*url + "latest") && (HTTP_CODE_OK == client.GET()))
    {
        String jsonString = client.getString();

        cJSON* release_json = cJSON_Parse(jsonString.c_str());

        if (cJSON_HasObjectItem(release_json, "body")
                && cJSON_HasObjectItem(release_json, "name")
                && cJSON_HasObjectItem(release_json, "assets"))
        {
            uint8_t update_idx = 0U;
            String changelog = String(cJSON_GetObjectItem(release_json, "body")->valuestring);
            
            changelog = changelog.substring(0, 512);
            changelog += "<br><br><a target=\"_blank\" href=\"" + String(cJSON_GetObjectItem(release_json, "html_url")->valuestring) + "\">Link to release</a>";
            changelog.replace("\n", "<br>");

            info.changelog = changelog;
            info.version = String(cJSON_GetObjectItem(release_json, "name")->valuestring);
            Log_Handler.println("Got release " + info.version);

            cJSON* assets = cJSON_GetObjectItem(release_json, "assets");

            for (uint8_t i = 0U; i < cJSON_GetArraySize(assets); i++)
            {
                cJSON* asset = cJSON_GetArrayItem(assets, i);

                if (cJSON_HasObjectItem(asset, "id"))
                {
                    String name = String(cJSON_GetObjectItem(asset, "name")->valuestring);
                    int asset_id = cJSON_GetObjectItem(asset, "id")->valueint;
                    //url.toLowerCase();
                    Log_Handler.print(name);
                    Log_Handler.print(" ID: ");
                    Log_Handler.println(asset_id);
                    if (name.indexOf("gbaHD-esp32.bin") != -1)
                    {
                        info.urls[update_idx].ota_part = OTA_ESP;
                        info.urls[update_idx].root_url = url;
                        info.urls[update_idx].id = asset_id;
                        update_idx++;
                    }
                    else if (name.indexOf("gbaHD-spiffs.bin") != -1)
                    {
                        info.urls[update_idx].ota_part = OTA_SPIFFS;
                        info.urls[update_idx].root_url = url;
                        info.urls[update_idx].id = asset_id;
                        update_idx++;
                    }
                    else if (name.indexOf("720") != -1)
                    {
                        info.urls[update_idx].ota_part = OTA_BS_720;
                        info.urls[update_idx].root_url = url;
                        info.urls[update_idx].id = asset_id;
                        update_idx++;
                    }
                    else if (name.indexOf("1080") != -1)
                    {
                        info.urls[update_idx].ota_part = OTA_BS_1080;
                        info.urls[update_idx].root_url = url;
                        info.urls[update_idx].id = asset_id;
                        update_idx++;
                    }

                }
                if (update_idx >= (sizeof(info.urls)/sizeof(info.urls[0])))
                {
                    break;
                }
            }
            for (;update_idx < (sizeof(info.urls)/sizeof(info.urls[0])); update_idx++)
            {
                info.urls[update_idx].ota_part = OTA_NONE;
            }
        }
        info.checked = true;
        cJSON_Delete(release_json);
    }
    else
    {
        Log_Handler.println("Failed to get url ");
        info.checked = false;
    }
    client.end();
    Log_Handler.println(ESP.getFreeHeap());
}

void OTA_Handler_Class::get_bitstream_update_info(Update_Info_p& info)
{
    if (!bs_update_info.checked)
    {
        refresh_update_info(bs_update_info, &OTA_BS_RELEASE_URL);
    }

    info = &bs_update_info;
}

void OTA_Handler_Class::get_esp_update_info(Update_Info_p& info)
{
    if (!release_update_info.checked)
    {
        String token;
        Preferences_Handler.getOTAToken(token);
        
        if (token != "")
        {
            Log_Handler.println("Getting ESP Testing");
            refresh_update_info(release_update_info, &OTA_PROD_TESTING_URL);
        }
        else
        {
            Log_Handler.println("Getting ESP Release");
            refresh_update_info(release_update_info, &OTA_PROD_RELEASE_URL);
        }
    }

    info = &release_update_info;
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
                Update.printError(Log_Handler);
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

    Log_Handler.printf("Initializing OTA for part %d\n", part.ota_part);
    Log_Handler.println(ESP.getFreeHeap());


    if (WiFi.isConnected())
    {
        Log_Handler.printf("AssetID: %d\n", part.id);
        if (!stream && part.root_url != nullptr)
        {
            String url = *part.root_url + "assets/" + part.id;
            Log_Handler.println("Trying to get url " + url);

            http_client.begin(url);
            initialize_client(http_client);
            http_client.addHeader("Accept", "application/octet-stream");
            int returncode = http_client.GET();

            Log_Handler.printf("ReturnCode %d\n", returncode);

            if ( (HTTP_CODE_OK == returncode))
            {
                stream = http_client.getStreamPtr();

                totalSize = http_client.getSize();
                remainingSize = totalSize;

                switch (part.ota_part)
                {
                    case OTA_ESP:
                        retval = Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH);
                        Log_Handler.println(Update.errorString());
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
                Log_Handler.println("Failed to get url " + url + " Error: " + returncode);
                retval = false;
                ota_state = OTA_STATE_FAILED;
            }
        }
    }
    return retval;
}

void OTA_Handler_Class::update(void)
{
    if ((ota_state == OTA_STATE_QUEUED) && (ota_queue_idx >= 0) && (ota_queue_length > ota_queue_idx))
    {
        Log_Handler.printf("Queue IDX %d is %d\n", ota_queue_idx, current_update->urls[ota_queue_idx].ota_part);

        if (current_update->urls[ota_queue_idx].ota_part != OTA_NONE)
        {
            current_part = current_update->urls[ota_queue_idx].ota_part;
            current_part_mapping = &current_update->urls[ota_queue_idx];
            if (initialize_ota(*current_part_mapping))
            {
                ota_state = OTA_STATE_RUNNING;
            }
        }
    }

    if ((OTA_STATE_RUNNING == ota_state) && WiFi.isConnected())
    {
        uint8_t buffer[1024] = { 0 };

        // read all data from server
        if ((stream->connected())
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
            
            Log_Handler.printf("State %d, Wifi: %d, Connection:: %d Stream: %d\n", ota_state, WiFi.isConnected(), stream->connected(), stream->available());
            Log_Handler.println("Remaining\t" + String(remainingSize) + "\t/ " + String(totalSize));


        }

        Log_Handler.printf("Free Heap: %d\n", ESP.getFreeHeap());

        if (!stream->connected() && remainingSize > 0)
        {
            http_client.setReuse(true);
            stream->stop();
            stream = nullptr;
            http_client.end();
            ota_state = OTA_STATE_FAILED;
        }
        else if (remainingSize == 0)
        {
            http_client.setReuse(true);
            stream->stop();
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
    uni_bluetooth_del_keys_safe();
    ota_queue_length = 2U;
    ota_queue_idx = 0U;
    ota_state = OTA_STATE_QUEUED;
}

void OTA_Handler_Class::update_latest_BS(void)
{
    current_update = &bs_update_info;
    uni_bluetooth_enable_new_connections_safe(false);
    uni_bluetooth_del_keys_safe();
    ota_queue_length = 2U;
    ota_queue_idx = 0U;
    ota_state = OTA_STATE_QUEUED;
    Log_Handler.println("Scheduling BS Updates.");
}

void OTA_Handler_Class::run(void)
{
    static bool firstRun = true;

    if (firstRun)
    {
        fallback_update();
        firstRun = false;
    }
    else if (WiFi.isConnected())
    {
        if ((!bs_update_info.checked) && (bs_retries < 10U))
        {
            refresh_update_info(bs_update_info, &OTA_BS_RELEASE_URL);


            if (!bs_update_info.checked)
            {
                bs_retries++;
            }
        }
        if ((!release_update_info.checked) && (esp_retries < 10U))
        {
            String token;
            Preferences_Handler.getOTAToken(token);
            
            if (token != "")
            {
                refresh_update_info(release_update_info, &OTA_PROD_TESTING_URL);
            }
            else
            {
                refresh_update_info(release_update_info, &OTA_PROD_RELEASE_URL);
            }

            if (!release_update_info.checked)
            {
                esp_retries++;
            }
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
        case OTA_STATE_FAILED:
            ws_client->text("-1");
            break;
        case OTA_STATE_NONE:
            break;
        default:
            break;
        }
    }
}

void OTA_Handler_Class::fallback_update(void)
{
    if (SD_MMC.begin())
    {
        Log_Handler.println("SD Card available! Checking for update...");
        unsigned char buffer[1024];
        int bytes_read;
        bool schedule_reboot = false;
        if (SD_MMC.exists(SD_ESP_FILE_NAME))
        {
            Log_Handler.println("ESP Update available. Updating");

            File SD_File = SD_MMC.open(SD_ESP_FILE_NAME, "r");
            Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH);
            Update.write(SD_File);
            Update.end();
            Log_Handler.println("Done!");
            Log_Handler.println("Removing Update File.");
            SD_File.close();
            SD_MMC.remove(SD_ESP_FILE_NAME);
            schedule_reboot = true;

        }
        if (SD_MMC.exists(SD_SPIFFS_FILE_NAME))
        {
            File SD_File = SD_MMC.open(SD_SPIFFS_FILE_NAME, "r");
            Log_Handler.println("SPIFFS Update available. Updating");
            LittleFS.end();
            Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS);
            Update.write(SD_File);
            Update.end();
            Log_Handler.println("Done!");
            Log_Handler.println("Removing Update File.");
            SD_File.close();
            SD_MMC.remove(SD_SPIFFS_FILE_NAME);
            schedule_reboot = true;
        }
        if (SD_MMC.exists(SD_720P_FILE_NAME))
        {
            File SD_File = SD_MMC.open(SD_720P_FILE_NAME, "r");
            File BS_File = LittleFS.open(BITSTREAM_720P_PATH, "w");
            
            Log_Handler.println("720P Update available. Updating");
            do
            {
                bytes_read = SD_File.read(buffer, sizeof(buffer)/sizeof(buffer[0]));
                BS_File.write(buffer, bytes_read);
            } while (bytes_read > 0);
            SD_File.close();
            BS_File.close();
            Log_Handler.println("Done!");
            Log_Handler.println("Removing Update File.");
            SD_MMC.remove(SD_720P_FILE_NAME);
            schedule_reboot = true;
        }
        if (SD_MMC.exists(SD_1080P_FILE_NAME))
        {
            File SD_File = SD_MMC.open(SD_1080P_FILE_NAME, "r");
            File BS_File = LittleFS.open(BITSTREAM_1080P_PATH, "w");
            
            Log_Handler.println("1080P Update available. Updating");
            do
            {
                bytes_read = SD_File.read(buffer, sizeof(buffer)/sizeof(buffer[0]));
                BS_File.write(buffer, bytes_read);
            } while (bytes_read > 0);
            SD_File.close();
            BS_File.close();
            Log_Handler.println("Done!");
            Log_Handler.println("Removing Update File.");
            SD_MMC.remove(SD_1080P_FILE_NAME);
            schedule_reboot = true;
        }
        if (SD_MMC.exists(SD_MEGA_FILE_NAME))
        {
            File SD_File = SD_MMC.open(SD_MEGA_FILE_NAME, "r");
            File BS_File = LittleFS.open(ATMEGA_SPIFFS_PATH, "w");
            
            Log_Handler.println("ATMega Update available. Updating");
            do
            {
                bytes_read = SD_File.read(buffer, sizeof(buffer)/sizeof(buffer[0]));
                BS_File.write(buffer, bytes_read);
            } while (bytes_read > 0);
            SD_File.close();
            BS_File.close();
            Log_Handler.println("Done!");
            Log_Handler.println("Removing Update File.");
            SD_MMC.remove(SD_MEGA_FILE_NAME);
            schedule_reboot = true;
        }
        SD_MMC.end();

        if (schedule_reboot)
        {
            ESP.restart(); 
        }
    }
    else
    {
        Log_Handler.println("Can not mount SD Card for Update");
    }
}

void OTA_Handler_Class::init(void)
{

    bs_update_info.changelog = "";
    bs_update_info.checked = false;
    bs_update_info.version = "";
    bs_update_info.available_parts = 0;

    release_update_info.changelog = "";
    release_update_info.checked = false;
    release_update_info.version = "";
    release_update_info.available_parts = 0;

    ws = new AsyncWebSocket("/ota");
    ws->onEvent(onWsEvent);
    Web_Handler.addWebSocket(ws);
    ws_client = nullptr;

}

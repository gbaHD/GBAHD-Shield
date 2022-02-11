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


#ifndef _OTA_HANDLER_H
#define _OTA_HANDLER_H

#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>


#define OTA_PROD_LittleFS_URL ( "https://cdn.discordapp.com/attachments/871683546031484928/938885413437837433/gbaHD-LittleFS.bin" )
#define OTA_PROD_APP_URL    ( "https://cdn.discordapp.com/attachments/871683546031484928/938885412888404018/gbaHD-esp32.bin" )

#define OTA_BS_720_URL      ( "https://raw.githubusercontent.com/zwenergy/gbaHD/master/bitstream/default_720p.bit" )
#define OTA_BS_1080_URL     ( "https://raw.githubusercontent.com/zwenergy/gbaHD/master/bitstream/default_1080p.bit" )

const String OTA_BS_RELEASE_URL = "https://api.github.com/repos/bbsan2k/gbaHD/releases/latest";
const String OTA_PROD_RELEASE_URL =  "https://api.github.com/repos/ManCloud/GBAHD-Shield/releases/latest" ;

enum OTA_Part
{
  OTA_NONE,
  OTA_ESP,
  OTA_SPIFFS,
  OTA_BS_720,
  OTA_BS_1080,
  OTA_ATMEGA,
  OTA_PART_MAX
};

struct OTA_Part_Mapping
{
    OTA_Part ota_part;
    String url;
};

struct Update_Info
{
  String version;
  OTA_Part_Mapping urls[5];
  uint8_t available_parts;
  String changelog;
  bool checked;
};


enum OTA_State
{
  OTA_STATE_NONE,
  OTA_STATE_QUEUED,
  OTA_STATE_RUNNING,
  OTA_STATE_COMPLETED
};

class OTA_Handler_Class
{
    public:
        bool get_update_available();
        void get_update_info(Update_Info& info);
        void update_bitstream(void);
        void full_update(void);
        void update_latest_BS(void);
        void init(void);
        void run(void);

  private:
        static void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        void write_to_current_output(uint8_t* buffer, size_t size, bool final=false);
        void refresh_update_info(Update_Info& info, const String& url);
        bool initialize_ota(OTA_Part_Mapping& part);
        void update(void);

        AsyncWebSocket* ws;
        AsyncWebSocketClient * ws_client;
        File outputFile;

        WiFiClient* stream = nullptr; 
        HTTPClient http_client;

        int remainingSize = -1;
        int totalSize = -1;
        OTA_Part current_part = OTA_NONE;
        OTA_State ota_state = OTA_STATE_NONE;
        OTA_Part ota_queue[4] = {OTA_NONE};
        uint8_t ota_queue_length = 0U;
        int8_t ota_queue_idx = -1;

        Update_Info bs_update_info;
        Update_Info release_update_info;
        Update_Info* current_update = nullptr;
        OTA_Part_Mapping* current_part_mapping = nullptr;

};

extern OTA_Handler_Class OTA_Handler;

#endif

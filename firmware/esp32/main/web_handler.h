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

#ifndef _WEB_HANDLER_H
#define _WEB_HANDLER_H

#include <FS.h>

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <stdint.h>

#define HTML_A      ( "A" )
#define HTML_B      ( "B" )
#define HTML_Y      ( "Y" )
#define HTML_X      ( "X" )
#define HTML_UP     ( "&#8679;" )
#define HTML_DOWN   ( "&#8681;" )
#define HTML_LEFT   ( "&#8678;" )
#define HTML_RIGHT  ( "&#8680;" )
#define HTML_L      ( "L" )
#define HTML_R      ( "R" )
#define HTML_TR_L   ( "Trigger L" )
#define HTML_TR_R   ( "Trigger R" )
#define HTML_SH_L   ( "Shoulder L" )
#define HTML_SH_R   ( "Shoulder R" )
#define HTML_START  ( "Start" ) 
#define HTML_SELECT ( "Select" )
#define HTML_SYSTEM ( "System" )
#define HTML_OSD    ( "OSD" )

#define WEB_TIMEOUT ( 180000000 )

class Web_Handler_Class {
    public:
        void addWebSocket(AsyncWebSocket* handler);
        bool isRunning(void);
        void init(void);
        void shutdown(void);
        void run(void);
    private:
        static void _handle404(AsyncWebServerRequest *request);
        static void _sendOK(AsyncWebServerRequest *request);
        static void handleSettings(AsyncWebServerRequest *request);
        static void handlePartitionUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
        static void handleLittleFSFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
        static void handleUploadDone(AsyncWebServerRequest *request);
        static void handleReboot(AsyncWebServerRequest *request);
        static void handleIndex(AsyncWebServerRequest *request);
        static void handleCSS(AsyncWebServerRequest *request);
        static void handleLogo(AsyncWebServerRequest *request);
        static void handleOTA(AsyncWebServerRequest *request);
        static void handleToken(AsyncWebServerRequest *request);
        static void handleBTReset(AsyncWebServerRequest *request);


        static String serial_ip(const String& var);
        static String ota_info(const String& var);


        static String build_option(uint16_t value, uint16_t mappedValue, String text);
        static String build_select_entry(uint16_t& inp_btn, uint16_t& mappedValue);
        static String build_update_done(bool success);


        static File fsUpload;
        static bool uploadSuccess;
        static AsyncWebServer _Aserver;
        static int64_t rebootTimer;
        static bool serverRunning;
};

extern Web_Handler_Class Web_Handler;

#endif
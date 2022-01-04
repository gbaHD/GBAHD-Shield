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

#include <WebServer.h>
#include <FS.h>

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

class Web_Handler_Class {
    public:
        void init(void);
        void run(void);
    private:
        static void _handle404();
        static void _sendOK();
        static void handleBluetooth();
        static void handlePartitionUpload();
        static void handleSPIFFSFileUpload();
        static void handleUploadDone();
        static void handleReboot();

        static String build_option(uint16_t value, uint16_t mappedValue, String text);
        static String build_select_entry(uint16_t& inp_btn, uint16_t& mappedValue);
        static String build_update_done(bool success);



        static File fsUpload;
        static bool uploadSuccess;
        static WebServer _server;
};

extern Web_Handler_Class Web_Handler;

#endif
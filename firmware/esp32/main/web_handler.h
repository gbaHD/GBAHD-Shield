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
        static void handleBitStreamUpload();
        static void handleESPUpload();
        static void handleSPIFFSUpload();
        static void handleBluetooth();

        static String build_option(uint16_t value, uint16_t mappedValue, String text);
        static String build_select_entry(uint16_t& inp_btn, uint16_t& mappedValue);

        static File fsUpload;
        static WebServer _server;
};

extern Web_Handler_Class Web_Handler;

#endif
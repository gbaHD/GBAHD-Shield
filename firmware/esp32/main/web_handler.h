#ifndef _WEB_HANDLER_H
#define _WEB_HANDLER_H

#include <WebServer.h>
#include <FS.h>



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

        static File fsUpload;
        static WebServer _server;
};

extern Web_Handler_Class Web_Handler;

#endif
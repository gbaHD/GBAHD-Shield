#ifndef _WIFI_HANDLER_H
#define _WIFI_HANDLER_H

#include "preferences_handler.h"


class Wifi_Handler_Class {
    public:
        void init();
        void update();
        void reset();
    private:
        void getSTACredentials(Wifi_Config& wifi_config);
        void connectWifiSTA(Wifi_Config& wifi_config);
};

extern Wifi_Handler_Class Wifi_Handler;

#endif
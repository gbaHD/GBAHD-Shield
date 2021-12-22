#ifndef _WIFI_HANDLER_H
#define _WIFI_HANDLER_H

class Wifi_Handler_Class {
    public:
        void init();
        void update();
        void reset();
    private:
        void getSTACredentials(String& ssid, String& password);
        bool saveWifiCredentials(String& ssid, String& password);
        void restoreWifiCredentials(String* ssid, String* password);
        void connectWifiSTA(String& ssid, String& password);
};

extern Wifi_Handler_Class Wifi_Handler;

#endif
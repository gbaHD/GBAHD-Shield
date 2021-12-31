#ifndef _PREFERENCES_HANDLER_H
#define _PREFERENCES_HANDLER_H

#include <Arduino.h>

#include "mega_handler.h"


enum 
{
    BT_INP_A      = 0U,
    BT_INP_B      = 1U,
    BT_INP_Y      = 2U,
    BT_INP_X      = 3U,
    BT_INP_UP     = 4U,
    BT_INP_DOWN   = 5U,
    BT_INP_LEFT   = 6U,
    BT_INP_RIGHT  = 7U,
    BT_INP_TR_L   = 8U,
    BT_INP_TR_R   = 9U,
    BT_INP_SH_L   = 10U,
    BT_INP_SH_R   = 11U,
    BT_INP_START  = 12U,
    BT_INP_SELECT = 13U,
    BT_INP_SYSTEM = 14U,
    BT_INP_MAX
};

typedef struct
{
    uint16_t mapping[BT_INP_MAX];
    bool enabled;
} Bluetooth_Config;

typedef struct 
{
    String ssid;
    String password; 
} Wifi_Config;


class Preferences_Handler_Class 
{
    public:
        void saveWifiCredentials(Wifi_Config& cfg);
        void getWifiCredentials(Wifi_Config& cfg)
        {
            cfg = wifi_config;
        }

        void saveBluetoothConfig(Bluetooth_Config& cfg);
        void getBluetoothConfig(Bluetooth_Config& cfg) {
            cfg = bt_config;
        }
        void reset();
        void init();

    private:
        void restoreBluetoothConfig();
        void restoreWifiCredentials();
        Wifi_Config wifi_config;
        Bluetooth_Config bt_config;


};

extern Preferences_Handler_Class Preferences_Handler;

#endif
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

enum
{
    BITSTREAM_720P  = 0U,
    BITSTREAM_1080P = 1U
};

typedef struct
{
    uint16_t bitstream;
} Settings;

typedef struct 
{
    String ssid;
    String password; 
    String hostname;
} Wifi_Config;


class Preferences_Handler_Class 
{
    public:
        void saveWifiCredentials(Wifi_Config& cfg);
        void getWifiCredentials(Wifi_Config& cfg)
        {
            cfg = this->wifi_config;
        }

        void saveBluetoothConfig(Bluetooth_Config& cfg);
        void getBluetoothConfig(Bluetooth_Config& cfg) {
            cfg = this->bt_config;
        }

        void saveSettings(Settings& settings);
        void getSettings(Settings& settings) {
            settings = this->settings;
        }

        void saveOTAToken(String& token);
        void getOTAToken(String& token)
        {
            token = this->token;
        }

        void run();

        void reset();
        void init();

    private:
        void restoreBluetoothConfig();
        void restoreWifiCredentials();
        void restoreSettings();
        void restoreOTAToken();
        Wifi_Config wifi_config;
        Bluetooth_Config bt_config;
        Settings settings;
        String token;


};

extern Preferences_Handler_Class Preferences_Handler;

#endif
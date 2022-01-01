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

#include <Preferences.h>

#include "preferences_handler.h"


void Preferences_Handler_Class::saveWifiCredentials(Wifi_Config& cfg)
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", cfg.ssid);
    preferences.putString("PASSWORD", cfg.password);
    preferences.end();
    wifi_config = cfg;
}


void Preferences_Handler_Class::restoreWifiCredentials()
{
  Preferences preferences;
  preferences.begin("GBAHD_WIFI");
  wifi_config.ssid = preferences.getString("SSID", "gbahd");
  wifi_config.password = preferences.getString("PASSWORD", "gbahdwifi");
  preferences.end();
}

void Preferences_Handler_Class::saveBluetoothConfig(Bluetooth_Config& cfg)
{
    Preferences preferences;
    preferences.begin("GBAHD_BTCONFIG");
    preferences.putUShort("D_UP",  cfg.mapping[BT_INP_UP]);
    preferences.putUShort("D_DWN", cfg.mapping[BT_INP_DOWN]);
    preferences.putUShort("D_LFT", cfg.mapping[BT_INP_LEFT]);
    preferences.putUShort("D_RGT", cfg.mapping[BT_INP_RIGHT]);
    preferences.putUShort("TR_L",  cfg.mapping[BT_INP_TR_L]);
    preferences.putUShort("TR_R",  cfg.mapping[BT_INP_TR_R]);
    preferences.putUShort("SH_L",  cfg.mapping[BT_INP_SH_L]);
    preferences.putUShort("SH_R",  cfg.mapping[BT_INP_SH_R]);
    preferences.putUShort("SYS",   cfg.mapping[BT_INP_SYSTEM]);
    preferences.putUShort("BTNA",  cfg.mapping[BT_INP_A]);
    preferences.putUShort("BTNB",  cfg.mapping[BT_INP_B]);
    preferences.getUShort("BTNX",  cfg.mapping[BT_INP_X]);
    preferences.getUShort("BTNY",  cfg.mapping[BT_INP_Y]);
    preferences.putUShort("STRT",  cfg.mapping[BT_INP_START]);
    preferences.putUShort("SEL",   cfg.mapping[BT_INP_SELECT]);
    preferences.putBool("ENABLED", cfg.enabled);
    preferences.end();
    bt_config = cfg;
}

void Preferences_Handler_Class::restoreBluetoothConfig()
{
    Preferences preferences;
    preferences.begin("GBAHD_BTCONFIG");
    bt_config.mapping[BT_INP_UP] = preferences.getUShort("D_UP", CTRL_IN_UP);
    bt_config.mapping[BT_INP_DOWN] = preferences.getUShort("D_DWN", CTRL_IN_DOWN);
    bt_config.mapping[BT_INP_LEFT] = preferences.getUShort("D_LFT", CTRL_IN_LEFT);
    bt_config.mapping[BT_INP_RIGHT] = preferences.getUShort("D_RGT", CTRL_IN_RIGHT);
    bt_config.mapping[BT_INP_TR_L] = preferences.getUShort("TR_L", CTRL_IN_L);
    bt_config.mapping[BT_INP_TR_R] = preferences.getUShort("TR_R", CTRL_IN_R);
    bt_config.mapping[BT_INP_SH_L] = preferences.getUShort("SH_L", CTRL_IN_L);
    bt_config.mapping[BT_INP_SH_R] = preferences.getUShort("SH_R", CTRL_IN_R);
    bt_config.mapping[BT_INP_SYSTEM] = preferences.getUShort("SYS", ENABLE_OSD);
    bt_config.mapping[BT_INP_A] = preferences.getUShort("BTNA", CTRL_IN_A);
    bt_config.mapping[BT_INP_B] = preferences.getUShort("BTNB", CTRL_IN_B);
    bt_config.mapping[BT_INP_X] = preferences.getUShort("BTNX", CTRL_IN_X);
    bt_config.mapping[BT_INP_Y] = preferences.getUShort("BTNY", CTRL_IN_Y);
    bt_config.mapping[BT_INP_START] = preferences.getUShort("STRT", CTRL_IN_START);
    bt_config.mapping[BT_INP_SELECT] = preferences.getUShort("SEL", CTRL_IN_SELECT);
    bt_config.enabled = preferences.getBool("ENABLED", true);
    preferences.end();    
}

void Preferences_Handler_Class::reset()
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", "");
    preferences.putString("PASSWORD", "");
    preferences.end();
}

void Preferences_Handler_Class::init()
{
    restoreBluetoothConfig();
    restoreWifiCredentials();
}


Preferences_Handler_Class Preferences_Handler;
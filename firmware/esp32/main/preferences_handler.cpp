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

#include "log_handler.h"

#include "preferences_handler.h"


static bool split(String& s, char separator, String& key, String& value)
{
    s.replace("\r", "");
    int idx = s.indexOf(separator);
    if (idx != -1)
    {
        key = s.substring(0, idx);
        value = s.substring(idx + 1, s.length());
        key.trim();
        value.trim();
        return true;
    }
    return false;
}


void Preferences_Handler_Class::saveWifiCredentials(Wifi_Config& cfg)
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", cfg.ssid);
    preferences.putString("PASSWORD", cfg.password);
    /* Save Hostname only if it is available */
    if (cfg.hostname.length())
    {
        preferences.putString("HOSTNAME", cfg.hostname);
    }
    preferences.end();
    wifi_config = cfg;
}


void Preferences_Handler_Class::restoreWifiCredentials()
{
  Preferences preferences;
  preferences.begin("GBAHD_WIFI");
  wifi_config.ssid = preferences.getString("SSID", "gbahd");
  wifi_config.password = preferences.getString("PASSWORD", "gbahdwifi");
  wifi_config.hostname = preferences.getString("HOSTNAME", "gbaHD");
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
    preferences.putUShort("BTNX",  cfg.mapping[BT_INP_X]);
    preferences.putUShort("BTNY",  cfg.mapping[BT_INP_Y]);
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
    bt_config.mapping[BT_INP_UP] =      preferences.getUShort("D_UP", CTRL_IN_UP);
    bt_config.mapping[BT_INP_DOWN] =    preferences.getUShort("D_DWN", CTRL_IN_DOWN);
    bt_config.mapping[BT_INP_LEFT] =    preferences.getUShort("D_LFT", CTRL_IN_LEFT);
    bt_config.mapping[BT_INP_RIGHT] =   preferences.getUShort("D_RGT", CTRL_IN_RIGHT);
    bt_config.mapping[BT_INP_TR_L] =    preferences.getUShort("TR_L", CTRL_IN_L);
    bt_config.mapping[BT_INP_TR_R] =    preferences.getUShort("TR_R", CTRL_IN_R);
    bt_config.mapping[BT_INP_SH_L] =    preferences.getUShort("SH_L", CTRL_IN_L);
    bt_config.mapping[BT_INP_SH_R] =    preferences.getUShort("SH_R", CTRL_IN_R);
    bt_config.mapping[BT_INP_SYSTEM] =  preferences.getUShort("SYS", ENABLE_OSD);
    bt_config.mapping[BT_INP_A] =       preferences.getUShort("BTNA", CTRL_IN_A);
    bt_config.mapping[BT_INP_B] =       preferences.getUShort("BTNB", CTRL_IN_B);
    bt_config.mapping[BT_INP_X] =       preferences.getUShort("BTNX", CTRL_IN_X);
    bt_config.mapping[BT_INP_Y] =       preferences.getUShort("BTNY", CTRL_IN_Y);
    bt_config.mapping[BT_INP_START] =   preferences.getUShort("STRT", CTRL_IN_START);
    bt_config.mapping[BT_INP_SELECT] =  preferences.getUShort("SEL", CTRL_IN_SELECT);
    bt_config.enabled =                 preferences.getBool("ENABLED", true);
    preferences.end();    
}

void Preferences_Handler_Class::saveOTAToken(String& token)
{
    Preferences preferences;
    preferences.begin("GBAHD_OTA");
    preferences.putString("TOKEN", token);
    preferences.end();
    this->token = token;
}

void Preferences_Handler_Class::saveSettings(Settings& settings)
{
    Preferences preferences;
    preferences.begin("GBAHD_SETTINGS");
    preferences.putUChar("BITSTREAM", settings.bitstream);
    preferences.end();
    this->settings = settings;
}

void Preferences_Handler_Class::restoreSettings()
{
    Preferences preferences;
    preferences.begin("GBAHD_SETTINGS");
    this->settings.bitstream = preferences.getUChar("BITSTREAM", BITSTREAM_720P);
    preferences.end();
}

void Preferences_Handler_Class::restoreOTAToken()
{

    Preferences preferences;
    preferences.begin("GBAHD_OTA");
    this->token = preferences.getString("TOKEN", "");
    preferences.end();
}

void Preferences_Handler_Class::run()
{
    if (Serial.available())
    {
        String input = Serial.readString();
        String key, value;
        input.trim();
        if (split(input, '=', key, value))
        {
            String config, sub_config;
            if (split(key, '.', config, sub_config))
            {
                config.toLowerCase();
                sub_config.toLowerCase();
                if (config == "wifi")
                {
                    if (sub_config == "ssid")
                    {
                        this->wifi_config.ssid = value;
                    }
                    else if (sub_config == "password")
                    {
                        this->wifi_config.password = value;
                    }
                    else if (sub_config == "hostname")
                    {
                        this->wifi_config.hostname = value;
                    }
                    this->saveWifiCredentials(this->wifi_config);
                }
            }

            Log_Handler.debugLine("Key " + key + " - Value: " + value);

        }
        else if (input == "reboot")
        {
            ESP.restart();
        }
    }
}

void Preferences_Handler_Class::reset()
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", "");
    preferences.putString("PASSWORD", "");

    /* Make sure to always use default Hostname after Reset */
    preferences.putString("HOSTNAME", "gbaHD");
    
    preferences.end();
}

void Preferences_Handler_Class::init()
{
    restoreBluetoothConfig();
    restoreWifiCredentials();
    restoreSettings();
    restoreOTAToken();
}


Preferences_Handler_Class Preferences_Handler;
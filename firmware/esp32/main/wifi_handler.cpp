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

#include <Arduino.h>
#include <WiFi.h>

#include <SD_MMC.h>

#include "wifi_handler.h"
#include "log_handler.h"

#define WIFI_SSID_SIZE    ( 32U )  // 32 length
#define WIFI_PW_SIZE      ( 64U )     // 64 length
#define WIFI_CREDS_FILE    "/wifi.cfg"


void Wifi_Handler_Class::getSTACredentials(Wifi_Config& wifi_config)
{
    Preferences_Handler.getWifiCredentials(wifi_config);
    Log_Handler.println(wifi_config.ssid);

    if (SD_MMC.begin())
    {
        Wifi_Config new_wifi_config;

        File wifi_creds = SD_MMC.open(WIFI_CREDS_FILE, "r", true);

        if (wifi_creds)
        {
            Log_Handler.println("Found creds file...");
            String line = wifi_creds.readStringUntil(0xA);
            do {
                int idx = -1;
                line.replace("\r", "");
                idx = line.indexOf("=");
                if (idx != -1)
                {
                    String key, value;
                    key = line.substring(0, idx);
                    value = line.substring(idx + 1, line.length());
                    key.trim();
                    value.trim();
                    if (key == "ssid")
                    {
                        new_wifi_config.ssid = value;
                    }
                    else if (key == "password")
                    {
                        new_wifi_config.password = value;
                    }
                }
                line = wifi_creds.readStringUntil(0xA);
            } while (line.length() > 0);
            wifi_creds.close();
        }
        else
        {
            Log_Handler.println("Didn't find any Wifi Creds file on SD-Card. Skipping Update.");
        }

        if ((wifi_config.ssid != new_wifi_config.ssid) || (wifi_config.password != new_wifi_config.password))
        {
            if ((new_wifi_config.ssid.length() < WIFI_SSID_SIZE) && (new_wifi_config.password.length() < WIFI_PW_SIZE))
            {
                Preferences_Handler.saveWifiCredentials(new_wifi_config);
                wifi_config = new_wifi_config;
            }
            else
            {
                Log_Handler.println("Error: Entered SSID or Password is too long and is potentially invalid.");
            }
        }
    }
    else
    {
        Log_Handler.println("Error: SD_MMC not available. Skip getting new Credentials...");
    }

    
}


void Wifi_Handler_Class::connectWifiSTA(Wifi_Config& wifi_config)
{
    WiFi.begin(wifi_config.ssid.c_str(), wifi_config.password.c_str());
}

void Wifi_Handler_Class::init()
{
    Wifi_Config wifi_config;

    WiFi.setHostname("gbaHD");

    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    //WiFi.setSleep(WIFI_PS_NONE);

    getSTACredentials(wifi_config);

    connectWifiSTA(wifi_config);

}

void Wifi_Handler_Class::update()
{
    static wl_status_t old_status = WL_NO_SHIELD;
    wl_status_t new_status = WiFi.status();
    if (old_status != new_status)
    {
        switch(new_status)
        {
            case WL_IDLE_STATUS:
            case WL_CONNECTED:
                Log_Handler.println("Connection established.");
                Log_Handler.print("IP:\t");
                Log_Handler.println(WiFi.localIP());
                WiFi.setTxPower(WIFI_POWER_19_5dBm);
               
                break;
            case WL_NO_SSID_AVAIL:
            case WL_CONNECT_FAILED:
            case WL_CONNECTION_LOST:
                Log_Handler.println("Connection not available.");
                break;
            case WL_SCAN_COMPLETED:
            case WL_DISCONNECTED:
            default:
            break;
        }

        old_status = new_status;
    }
}



Wifi_Handler_Class Wifi_Handler;
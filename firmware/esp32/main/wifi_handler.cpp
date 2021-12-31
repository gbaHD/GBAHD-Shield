#include <Arduino.h>
#include <WiFi.h>
#include <SD_MMC.h>
#include <cJSON.h>

#include "wifi_handler.h"

#define WIFI_SSID_SIZE    ( 32U )  // 32 length
#define WIFI_PW_SIZE      ( 64U )     // 64 length
#define WIFI_CREDS_FILE    "/wifi.cfg"


void Wifi_Handler_Class::getSTACredentials(Wifi_Config& wifi_config)
{
    Preferences_Handler.getWifiCredentials(wifi_config);
    Serial.println(wifi_config.ssid);

    if (SD_MMC.begin())
    {
        Wifi_Config new_wifi_config;

        File wifi_creds = SD_MMC.open(WIFI_CREDS_FILE, "r", true);

        if (wifi_creds)
        {
            Serial.println("Found creds file...");
            if (wifi_creds.size() < 1024U)
            {
                cJSON *creds_json = NULL;

                {
                    char buffer[1024U] = { 0 };
                    wifi_creds.readBytes(buffer, (sizeof(buffer)/sizeof(buffer[0])));
                    
                    creds_json = cJSON_Parse(buffer);
                    Serial.println("Loaded new creds...");
                }

                if (NULL != creds_json)
                {
                    {
                        cJSON* ssid = cJSON_GetObjectItem(creds_json, "ssid");
                        if (cJSON_IsString(ssid) && (ssid->valuestring != NULL))
                        {
                            new_wifi_config.ssid = ssid->valuestring;
                            Serial.println("Creds File SSID: " + new_wifi_config.ssid);
                        }
                    }
                    {
                        cJSON* password = cJSON_GetObjectItem(creds_json, "password");
                        if (cJSON_IsString(password) && (password->valuestring != NULL))
                        {
                            new_wifi_config.password = password->valuestring;
                        }
                    }
                    cJSON_Delete(creds_json);

                }
                else
                {
                    Serial.println("WiFi Creds JSON was invalid");
                }
            }
            else
            {
                Serial.println("WiFi Creds JSON too big");
            }
            wifi_creds.close();
        }
        else
        {
            Serial.println("Didn't find any Wifi Creds file on SD-Card. Skipping Update.");
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
                Serial.println("Error: Entered SSID or Password is too long and is potentially invalid.");
            }
        }
    }
    else
    {
        Serial.println("Error: SD_MMC not available. Skip getting new Credentials...");
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
                Serial.println("Connection established.");
                Serial.print("IP:\t");
                Serial.println(WiFi.localIP());
                break;
            case WL_NO_SSID_AVAIL:
            case WL_CONNECT_FAILED:
            case WL_CONNECTION_LOST:
                Serial.println("Connection not available.");
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
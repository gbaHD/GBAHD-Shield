#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <SD_MMC.h>
#include <cJSON.h>

#include "wifi_handler.h"


#define WIFI_SSID_SIZE    ( 32U )  // 32 length
#define WIFI_PW_SIZE      ( 64U )     // 64 length
#define WIFI_CREDS_FILE     ( "/wifi_credentials.json" )


void Wifi_Handler_Class::getSTACredentials(String& ssid, String& password)
{
    restoreWifiCredentials(&ssid, &password);
    Serial.println(ssid);

    if (SD_MMC.begin())
    {
        String newSSID, newPassword;

        if (SD_MMC.exists(WIFI_CREDS_FILE))
        {
            File wifi_creds = SD_MMC.open(WIFI_CREDS_FILE, "r");
            if (wifi_creds.size() < 1024U)
            {
                cJSON *creds_json = NULL;

                {
                    char buffer[1024U] = { 0 };
                    wifi_creds.readBytes(buffer, (sizeof(buffer)/sizeof(buffer[0])));
                    
                    creds_json = cJSON_Parse(buffer);
                }

                if (NULL != creds_json)
                {
                    {
                        cJSON* ssid = cJSON_GetObjectItem(creds_json, "ssid");
                        if (cJSON_IsString(ssid) && (ssid->valuestring != NULL))
                        {
                            newSSID = ssid->valuestring;
                        }
                    }
                    {
                        cJSON* password = cJSON_GetObjectItem(creds_json, "password");
                        if (cJSON_IsString(password) && (password->valuestring != NULL))
                        {
                            newPassword = password->valuestring;
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
        if ((ssid != newSSID) || (password != newPassword))
        {
            saveWifiCredentials(newSSID, newPassword);
            ssid = newSSID;
            password = newPassword;
        }
    }
    else
    {
        Serial.println("Error: SD_MMC not available. Skip getting new Credentials...");
    }

}


bool Wifi_Handler_Class::saveWifiCredentials(String& ssid, String& password)
{
  bool retval = true;

  if ((ssid.length() < WIFI_SSID_SIZE) && (password.length() < WIFI_PW_SIZE))
  {
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", ssid);
    preferences.putString("PASSWORD", password);
    preferences.end();
    retval = true;
  }
  else
  {
    retval = false;
    Serial.println("Entered SSID or Password is too long and is potentially invalid.");
  }

  return retval;
}


void Wifi_Handler_Class::restoreWifiCredentials(String* ssid, String* password)
{
  Preferences preferences;
  preferences.begin("GBAHD_WIFI");
  *ssid = preferences.getString("SSID", "gbahd");
  *password = preferences.getString("PASSWORD", "gbahdwifi");

  preferences.end();
}

void Wifi_Handler_Class::connectWifiSTA(String& ssid, String& password)
{
    WiFi.mode(WIFI_STA);  

    WiFi.begin(ssid.c_str(), password.c_str());
}

void Wifi_Handler_Class::init()
{
    String ssid;
    String password;

    WiFi.setHostname("gbaHD");

    getSTACredentials(ssid, password);

    connectWifiSTA(ssid, password);
}

void Wifi_Handler_Class::update()
{
	static wl_status_t last_status = WL_DISCONNECTED;
	wl_status_t curr_status = WiFi.status();
	if(last_status != curr_status) {
		last_status = curr_status;
		switch(curr_status)
		{
		case WL_IDLE_STATUS:
		case WL_CONNECTED:
			Serial.println("Connection established.");
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
	}
}

void Wifi_Handler_Class::reset()
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", "");
    preferences.putString("PASSWORD", "");
    preferences.end();
}

Wifi_Handler_Class Wifi_Handler;

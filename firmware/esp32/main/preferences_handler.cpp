#include <Preferences.h>

#include "preferences_handler.h"


void Preferences_Handler_Class::saveWifiCredentials(String& ssid, String& password)
{
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", ssid);
    preferences.putString("PASSWORD", password);
    preferences.end();
}


void Preferences_Handler_Class::restoreWifiCredentials(String& ssid, String& password)
{
  Preferences preferences;
  preferences.begin("GBAHD_WIFI");
  ssid = preferences.getString("SSID", "gbahd");
  password = preferences.getString("PASSWORD", "gbahdwifi");

  preferences.end();
}


void Preferences_Handler_Class::saveBluetoothConfig(bool& bluetooth_enable, Button_Mapping& mapping)
{
    Preferences preferences;
    preferences.begin("GBAHD_BTCONFIG");
    preferences.putUShort("D_UP", mapping.dpad_up);
    preferences.putUShort("D_DWN", mapping.dpad_down);
    preferences.putUShort("D_LFT", mapping.dpad_left);
    preferences.putUShort("D_RGT", mapping.dpad_right);
    preferences.putUShort("TR_L", mapping.trigger_l);
    preferences.putUShort("TR_R", mapping.trigger_r);
    preferences.putUShort("SH_L", mapping.shoulder_l);
    preferences.putUShort("SH_R", mapping.shoulder_r);
    preferences.putUShort("SYS", mapping.system);
    preferences.putUShort("STRT", mapping.start);
    preferences.putUShort("SEL", mapping.select);
    preferences.end();
}

void Preferences_Handler_Class::restoreBluetoothConfig(bool& bluetooth_enable, Button_Mapping& mapping)
{
    Preferences preferences;
    preferences.begin("GBAHD_BTCONFIG");
    preferences.getUShort("D_UP", mapping.dpad_up);
    preferences.getUShort("D_DWN", mapping.dpad_down);
    preferences.getUShort("D_LFT", mapping.dpad_left);
    preferences.getUShort("D_RGT", mapping.dpad_right);
    preferences.getUShort("TR_L", mapping.trigger_l);
    preferences.getUShort("TR_R", mapping.trigger_r);
    preferences.getUShort("SH_L", mapping.shoulder_l);
    preferences.getUShort("SH_R", mapping.shoulder_r);
    preferences.getUShort("SYS", mapping.system);
    preferences.getUShort("STRT", mapping.start);
    preferences.getUShort("SEL", mapping.select);
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

Preferences_Handler_Class Preferences_Handler;
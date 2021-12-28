#ifndef _PREFERENCES_HANDLER_H
#define _PREFERENCES_HANDLER_H

#include <Arduino.h>

typedef struct
{
    uint16_t dpad_up;
    uint16_t dpad_down;
    uint16_t dpad_left;
    uint16_t dpad_right;
    uint16_t trigger_l;
    uint16_t trigger_r;
    uint16_t shoulder_l;
    uint16_t shoulder_r;
    uint16_t system;
    uint16_t start;
    uint16_t select;
} Button_Mapping;

class Preferences_Handler_Class 
{
    public:
        void saveWifiCredentials(String& ssid, String& password);
        void restoreWifiCredentials(String& ssid, String& password);
        void saveBluetoothConfig(bool& bluetooth_enable, Button_Mapping& mapping);
        void restoreBluetoothConfig(bool& bluetooth_enable, Button_Mapping& mapping);
        void reset();

    private:



};

extern Preferences_Handler_Class Preferences_Handler;

#endif
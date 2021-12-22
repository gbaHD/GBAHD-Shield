
#include <Arduino.h>
#include <Wire.h>
#include <Bluepad32.h>
#include <SPIFFS.h>

#include "mega_handler.h"


#define MAP_BUTTON(INPUT, OUTPUT, I_MASK, O_MASK) (OUTPUT |= ( INPUT & I_MASK ) ? O_MASK : 0)

#define GBA_OUT_A      (0x010)
#define GBA_OUT_B      (0x020)
#define GBA_OUT_UP     (0x001)
#define GBA_OUT_DOWN   (0x002)
#define GBA_OUT_LEFT   (0x004)
#define GBA_OUT_RIGHT  (0x008)
#define GBA_OUT_L      (0x040)
#define GBA_OUT_R      (0x080)
#define GBA_OUT_START  (0x100)
#define GBA_OUT_SELECT (0x200)

#define MEGA_BL_ADDRESS             (0x29)
#define MEGA_VERSION_HASH_OFFSET    (0x100) // <-- This needs to be updated once we know the location
#define MEGA_PAGE_SIZE              (0x80)  // <-- Page Size of ATMega328P

#define SDA_PIN         (32)
#define SCL_PIN         (33)

GamepadPtr Mega_Handler_Class::controller;


void Mega_Handler_Class::onConnectedGamepad(GamepadPtr gp) 
{
  controller = gp;
  Serial.println("CALLBACK: Controller is connected!");
}

void Mega_Handler_Class::onDisconnectedGamepad(GamepadPtr gp) 
{
  Serial.println("CALLBACK: Controller is disconnected!");
  controller = nullptr;
}

void Mega_Handler_Class::update_controller()
{
    uint16_t newOutputs = 0U;

    BP32.update();

    if (controller && controller->isConnected()) 
    {
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_UP, GBA_OUT_UP);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_DOWN, GBA_OUT_DOWN);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_LEFT, GBA_OUT_LEFT);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_RIGHT, GBA_OUT_RIGHT);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_A, GBA_OUT_A);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_B, GBA_OUT_B);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_L, GBA_OUT_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_L, GBA_OUT_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_R, GBA_OUT_R);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_R, GBA_OUT_R);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_HOME, GBA_OUT_START);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_BACK, GBA_OUT_SELECT);
    }

    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(reinterpret_cast<uint8_t*>(&newOutputs), sizeof(uint16_t));
    Wire.endTransmission();

    if (newOutputs != 0)
    {
        Serial.print("Current Controls: ");
        Serial.println(newOutputs, BIN);
    }
}

void Mega_Handler_Class::get_update_version(String& version)
{
    char hash[8] = {0};
    File atmelFile = SPIFFS.open("/atmel.hex", "r");

    if (atmelFile)
    {
        atmelFile.seek(MEGA_VERSION_HASH_OFFSET);

        atmelFile.readBytes(hash, 7);
    }
    version += hash;

    atmelFile.close();
}

void Mega_Handler_Class::get_mega_version(String& version)
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x02);   // <-- Command
    Wire.write(0x01);   // <-- Use FLASH
    Wire.write(0xFF & (MEGA_VERSION_HASH_OFFSET >> 8));
    Wire.write(0xFF & MEGA_VERSION_HASH_OFFSET);
    Wire.endTransmission();
    Wire.requestFrom(MEGA_BL_ADDRESS, 7); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        version += static_cast<char>(Wire.read());
    }

    Wire.endTransmission();
}

void Mega_Handler_Class::stop_bootloader()
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
}

void Mega_Handler_Class::start_application()
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x01);
    Wire.write(0x80);
    Wire.endTransmission();
}

void Mega_Handler_Class::update_mega()
{
    String oldVersion = "", newVersion = "";
    get_mega_version(oldVersion);
    get_update_version(newVersion);
    if (oldVersion != newVersion)
    {
        File atmelFile = SPIFFS.open("/atmel.hex", "r");
        uint16_t write_address = 0;

        char buffer[MEGA_PAGE_SIZE] = {0};
        
        while (atmelFile.readBytes(buffer, MEGA_PAGE_SIZE) > 0)
        {
            Wire.beginTransmission(MEGA_BL_ADDRESS);
            Wire.write(0x02);
            Wire.write(0x01);

            Wire.write(0xFF & (write_address >> 8));
            Wire.write(0xFF & write_address);

            for (uint8_t idx = 0U; idx < MEGA_PAGE_SIZE; idx++)
            {
                Wire.write(buffer[idx]);
            }

            Wire.endTransmission();

            write_address += MEGA_PAGE_SIZE;
        }

        atmelFile.close();
    }
}


void Mega_Handler_Class::init()
{
    String fv = BP32.firmwareVersion();
    Serial.print("Bluepad32 Firmware: ");
    Serial.println(fv);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    Wire.begin(SDA_PIN, SCL_PIN);

    stop_bootloader();

    update_mega();

    start_application();
}

void Mega_Handler_Class::update()
{
    update_controller();
}

Mega_Handler_Class Mega_Handler;
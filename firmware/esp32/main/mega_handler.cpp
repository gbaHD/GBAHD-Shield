
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
#define MEGA_VERSION_HASH_OFFSET    (0x200) // <-- This needs to be updated once we know the location
#define MEGA_PAGE_SIZE              (0x80)  // <-- Page Size of ATMega328P

#define SDA_PIN         (32)
#define SCL_PIN         (33)

GamepadPtr Mega_Handler_Class::controller;

const String avr_upd_bin = "/atmega.bin";

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
    Wire.flush();

    if (newOutputs != 0)
    {
        Serial.print("Current Controls: ");
        Serial.println(newOutputs, BIN);
    }
}

void Mega_Handler_Class::get_update_version(String& version)
{
    char hash[8] = {0};
    File atmelFile = SPIFFS.open(avr_upd_bin, "r");

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
    uint8_t cmd[4] = {0x02, 0x01, ((MEGA_VERSION_HASH_OFFSET >> 8) & 0xFF), MEGA_VERSION_HASH_OFFSET & 0xFF};
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission();
    Wire.requestFrom(MEGA_BL_ADDRESS, 7); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        version += static_cast<char>(Wire.read());
    }
}

void Mega_Handler_Class::stop_bootloader()
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.flush();
}

void Mega_Handler_Class::start_application()
{
	Serial.println("Starting application");
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x01);
    Wire.write(0x80);
    Wire.endTransmission();
    Wire.flush();
}

void Mega_Handler_Class::update_mega()
{
    String oldVersion = "", newVersion = "";
    get_mega_version(oldVersion);
    get_update_version(newVersion);
    if (oldVersion != newVersion)
    {
        Serial.print("New version detected... Old version: ");
        Serial.print(oldVersion);
        Serial.print("New version: ");
        Serial.println(newVersion);

        File atmelFile = SPIFFS.open(avr_upd_bin, "r");
        uint16_t write_address = 0;

        Serial.println("Updating Shield...");
        uint8_t buffer[MEGA_PAGE_SIZE] = {0};
        
        while (atmelFile.read(buffer, MEGA_PAGE_SIZE) > 0)
        {
            Serial.print("Writing at ");
            Serial.println(write_address);
            Wire.beginTransmission(MEGA_BL_ADDRESS);
            Wire.write(0x02);
            Wire.write(0x01);
            Wire.write(0xFF & (write_address >> 8));
            Wire.write(0xFF & write_address);

            Wire.write(buffer, sizeof(buffer));

            Wire.endTransmission();
            Serial.println();

            write_address += MEGA_PAGE_SIZE;
        }

    	Serial.println("Update: Done");
        atmelFile.close();
    } else {
    	Serial.println("Version Match: No Update!");
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

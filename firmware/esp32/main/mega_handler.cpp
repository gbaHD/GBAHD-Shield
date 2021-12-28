
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

#define CTRL_IN_A      (0x008)
#define CTRL_IN_B      (0x800)
#define CTRL_IN_Y      (0x400)
#define CTRL_IN_X      (0x004)
#define CTRL_IN_UP     (0x080)
#define CTRL_IN_DOWN   (0x040)
#define CTRL_IN_LEFT   (0x020)
#define CTRL_IN_RIGHT  (0x010)
#define CTRL_IN_L      (0x002)
#define CTRL_IN_R      (0x001)
#define CTRL_IN_START  (0x100)
#define CTRL_IN_SELECT (0x200)

#define ENABLE_OSD     (CTRL_IN_UP | CTRL_IN_R | CTRL_IN_L | CTRL_IN_SELECT)


#define MEGA_BL_ADDRESS             (0x29)
#define MEGA_VERSION_HASH_OFFSET    (0x200) // <-- This needs to be updated once we know the location
#define MEGA_PAGE_SIZE              (0x80)  // <-- Page Size of ATMega328P
#define MEGA_RESPONSE_TIMEOUT       (10)

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
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_UP, CTRL_IN_UP);
        MAP_BUTTON(((controller->axisY() < -80) ? DPAD_UP : 0), newOutputs, DPAD_UP, CTRL_IN_UP);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_DOWN, CTRL_IN_DOWN);
        MAP_BUTTON(((controller->axisY() > 80) ? DPAD_DOWN : 0), newOutputs, DPAD_DOWN, CTRL_IN_DOWN);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_LEFT, CTRL_IN_LEFT);
        MAP_BUTTON(((controller->axisX() < -80) ? DPAD_LEFT : 0), newOutputs, DPAD_LEFT, CTRL_IN_LEFT);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_RIGHT, CTRL_IN_RIGHT);
        MAP_BUTTON(((controller->axisX() > 80) ? DPAD_RIGHT : 0), newOutputs, DPAD_RIGHT, CTRL_IN_RIGHT);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_B, CTRL_IN_A);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_A, CTRL_IN_B);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_Y, CTRL_IN_X);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_X, CTRL_IN_Y);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_L, CTRL_IN_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_L, CTRL_IN_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_R, CTRL_IN_R);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_R, CTRL_IN_R);

        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_HOME, CTRL_IN_START);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_BACK, CTRL_IN_SELECT);

        if (controller->miscSystem())
        {
            newOutputs = ENABLE_OSD;
        }

        Wire.beginTransmission(MEGA_BL_ADDRESS);
        Wire.write(reinterpret_cast<uint8_t*>(&newOutputs), sizeof(uint16_t));
        Wire.endTransmission();
        Wire.flush();


        if (newOutputs != 0)
        {
        	Serial.print("Current Controls: 0x");
        	Serial.println(newOutputs, HEX);
        }
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

    Serial.print("New version:\t");

    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(version.c_str()[i], HEX);
    }

    Serial.println("");
}

void Mega_Handler_Class::get_mega_version(String& version)
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    uint8_t cmd[4] = {0x02, 0x01, ((MEGA_VERSION_HASH_OFFSET >> 8) & 0xFF), MEGA_VERSION_HASH_OFFSET & 0xFF};
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 7); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        version += static_cast<char>(Wire.read());
    }

    Serial.print("Old version:\t");
    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(version.c_str()[i], HEX);
    }

    Serial.println("");
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

uint16_t Mega_Handler_Class::get_twi_flash_bytes(uint16_t address, uint8_t* buffer, size_t size)
{
    uint8_t cmd[4] = {0x02, 0x01, highByte(address), lowByte(address)};
    uint16_t timeout = 0U;
    uint16_t bytes_read = 0U;
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, size); // <-- Request only 7 Byte for short commit hash

    while ((!Wire.available()) && (timeout < MEGA_RESPONSE_TIMEOUT)) 
    {
        Serial.println("Waiting for TWI response");
        if (++timeout > MEGA_RESPONSE_TIMEOUT)
        {
            Serial.println("Timeout!");
            break;
        }
    }

    for(; (bytes_read < size) && Wire.available(); bytes_read++)
    {
        buffer[bytes_read] = Wire.read();
    }

    return bytes_read;
}

bool Mega_Handler_Class::verify_flash(uint16_t address, const uint8_t* buffer, size_t size)
{
    uint8_t cmd[4] = {0x02, 0x01, highByte(address), lowByte(address)};
    uint16_t timeout = 0U;
    bool ret = true;

    Serial.print("Waiting for TWI response");

    do {
        Serial.print(".");
        
        Wire.beginTransmission(MEGA_BL_ADDRESS);
        Wire.write(cmd, sizeof(cmd)/sizeof(uint8_t));   // <-- Command Read
        Wire.endTransmission(false);
        Wire.requestFrom(MEGA_BL_ADDRESS, size);

        delay(100);
        if (++timeout > MEGA_RESPONSE_TIMEOUT)
        {
            ret = false;
            Serial.println("Timeout!");
            break;
        }
    } while ((!Wire.available()) && (timeout < MEGA_RESPONSE_TIMEOUT));

    for(uint16_t i = 0U; (i < size) && ret; i++)
    {
        if (buffer[i] != Wire.read())
        {
            ret = false;
        }
    }



    return ret;
}

void Mega_Handler_Class::print_chip_info()
{
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 8);
    Serial.print("Chip Info: ");
    for (uint8_t i = 0U; i < 8; i++)
    {
        Serial.print(i);
        Serial.print(" - ");
        Serial.println(static_cast<char>(Wire.read()), HEX);
    }
    Serial.println("");
}

void Mega_Handler_Class::update_mega()
{
    String oldVersion = "", newVersion = "";
    get_mega_version(oldVersion);
    get_update_version(newVersion);
    if (oldVersion != newVersion)
    {
        print_chip_info();

        File atmelFile = SPIFFS.open(avr_upd_bin, "r");
        uint16_t write_address = 0;

        Serial.println("Updating Shield...");
        uint8_t buffer[MEGA_PAGE_SIZE+4] = {0};
        buffer[0] = 0x02;
        buffer[1] = 0x01;
        while (atmelFile.read(&buffer[4], MEGA_PAGE_SIZE) > 0)
        {
            Serial.print("Writing at ");
            Serial.println(write_address, HEX);
            buffer[2] = highByte(write_address);
            buffer[3] = lowByte(write_address);

            Wire.beginTransmission(MEGA_BL_ADDRESS);
            if(Wire.write(buffer, sizeof(buffer)) != sizeof(buffer))
            	Serial.println("Page not sent completely!");

            Wire.endTransmission();


            delay(100);

            Serial.println("Flash verify ...");
            if (!verify_flash(write_address, &buffer[4], MEGA_PAGE_SIZE))
            {
                Serial.println("verify failed!");
            } else {
                Serial.println("verify passed!");
            }
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

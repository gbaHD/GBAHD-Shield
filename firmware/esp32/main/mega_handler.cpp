
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
#include <Wire.h>
#include <Bluepad32.h>
#include <SPIFFS.h>
#include <mbedtls/sha1.h>

#include "mega_handler.h"
#include "preferences_handler.h"



#define MAP_BUTTON(INPUT, OUTPUT, I_MASK, O_MASK) (OUTPUT |= ( INPUT & I_MASK ) ? O_MASK : 0)

#define MEGA_BL_ADDRESS             (0x29)
#define MEGA_VERSION_HASH_OFFSET    (0x200) // <-- This needs to be updated once we know the location
#define MEGA_PAGE_SIZE              (0x80)  // <-- Page Size of ATMega328P
#define MEGA_RESPONSE_TIMEOUT       (10)

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
    Bluetooth_Config bt_config;
    Preferences_Handler.getBluetoothConfig(bt_config);

    BP32.update();

    if (controller && controller->isConnected()) 
    {
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_UP,  bt_config.mapping[BT_INP_UP]);
        MAP_BUTTON(((controller->axisY() < -80) ? DPAD_UP : 0), newOutputs, DPAD_UP, bt_config.mapping[BT_INP_UP]);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_DOWN, bt_config.mapping[BT_INP_DOWN]);
        MAP_BUTTON(((controller->axisY() > 80) ? DPAD_DOWN : 0), newOutputs, DPAD_DOWN, bt_config.mapping[BT_INP_DOWN]);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_LEFT, bt_config.mapping[BT_INP_LEFT]);
        MAP_BUTTON(((controller->axisX() < -80) ? DPAD_LEFT : 0), newOutputs, DPAD_LEFT, bt_config.mapping[BT_INP_LEFT]);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_RIGHT, bt_config.mapping[BT_INP_RIGHT]);
        MAP_BUTTON(((controller->axisX() > 80) ? DPAD_RIGHT : 0), newOutputs, DPAD_RIGHT, bt_config.mapping[BT_INP_RIGHT]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_B, bt_config.mapping[BT_INP_B]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_A, bt_config.mapping[BT_INP_A]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_Y, bt_config.mapping[BT_INP_Y]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_X, bt_config.mapping[BT_INP_X]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_L, bt_config.mapping[BT_INP_TR_L]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_L, bt_config.mapping[BT_INP_SH_L]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_R, bt_config.mapping[BT_INP_TR_R]);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_R, bt_config.mapping[BT_INP_SH_R]);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_SYSTEM, bt_config.mapping[BT_INP_SYSTEM]);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_HOME, bt_config.mapping[BT_INP_START]);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_BACK, bt_config.mapping[BT_INP_SELECT]);


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
    unsigned char hash[20] = {0};
    File atmelFile = SPIFFS.open(ATMEGA_SPIFFS_PATH, "r");

    if (atmelFile)
    {
        mbedtls_sha1_context ctx;
        mbedtls_sha1_init(&ctx);
        mbedtls_sha1_starts(&ctx);
        unsigned char buffer[MEGA_PAGE_SIZE];

        size_t bytes_read = 0U;
        do
        {
            bytes_read = atmelFile.readBytes(reinterpret_cast<char*>(buffer), MEGA_PAGE_SIZE);
            mbedtls_sha1_update(&ctx, (buffer), bytes_read);
        } while (bytes_read > 0U);
        mbedtls_sha1_finish(&ctx, hash);
    }

    atmelFile.close();

    Serial.print("New version:\t");

    for (uint8_t i = 0; i < 20; i++)
    {
        version += String(hash[i], HEX);
    }

    Serial.println(version);
}

void Mega_Handler_Class::get_mega_version(String& version)
{
    unsigned char hash[20] = {0};
    uint8_t cmd[4] = {0x02, 0x02, 0x00, 0x00};

    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 20); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        Wire.readBytes(hash, 20);
    }

    Serial.print("Old version:\t");
    for (uint8_t i = 0; i < 20; i++)
    {
        version += String(hash[i], HEX);
    }

    Serial.println(version);
}

void Mega_Handler_Class::restart_shield()
{
    uint16_t restart = RESET_SHIELD;
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(reinterpret_cast<uint8_t*>(&restart), sizeof(uint16_t));
    Wire.endTransmission();
    Wire.flush();
    delay(50);
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

    Serial.println("Waiting for TWI response");

    do  
    {
        Serial.print(".");
        
        Wire.beginTransmission(MEGA_BL_ADDRESS);

        Wire.write(cmd, sizeof(cmd));   // <-- Command Read
        Wire.endTransmission(false);

        Wire.requestFrom(MEGA_BL_ADDRESS, size); // <-- Request only 7 Byte for short commit hash
        if (++timeout > MEGA_RESPONSE_TIMEOUT)
        {
            Serial.println("Timeout!");
            break;
        }

    } while ((!Wire.available()) && (timeout < MEGA_RESPONSE_TIMEOUT));

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

bool Mega_Handler_Class::get_chip_info()
{
    bool ret = false;
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 8);
    Serial.print("Chip Info: ");
    if (Wire.available())
    {
        for (uint8_t i = 0U; i < 8; i++)
        {
            Serial.print(i);
            Serial.print(" - ");
            Serial.println(static_cast<char>(Wire.read()), HEX);
        }
        Serial.println("");
        ret = true;
    }
    else
    {
        Serial.println("TWI is not answering...");
        ret = false;
    }
    return ret;
}

void Mega_Handler_Class::update_mega()
{
    String oldVersion = "", newVersion = "";

    if (!get_chip_info())
    {
        restart_shield();
    }
    
    get_mega_version(oldVersion);
    get_update_version(newVersion);

    if (oldVersion != newVersion)
    {
        mbedtls_sha1_context ctx;
        mbedtls_sha1_init(&ctx);
        mbedtls_sha1_starts(&ctx);

        {
            File atmelFile = SPIFFS.open(ATMEGA_SPIFFS_PATH, "r");
            uint16_t write_address = 0;
            uint8_t bytes_read = 0U;
            uint8_t buffer[MEGA_PAGE_SIZE+4] = {0};

            Serial.println("Updating Shield...");

            buffer[0] = 0x02;
            buffer[1] = 0x01;

            bytes_read = atmelFile.read(&buffer[4], MEGA_PAGE_SIZE);

            while (bytes_read > 0)
            {
                Serial.print("Writing at ");
                Serial.println(write_address, HEX);
                buffer[2] = highByte(write_address);
                buffer[3] = lowByte(write_address);

                Wire.beginTransmission(MEGA_BL_ADDRESS);
                if(Wire.write(buffer, sizeof(buffer)) != sizeof(buffer))
                    Serial.println("Page not sent completely!");

                Wire.endTransmission();

                mbedtls_sha1_update(&ctx, &buffer[4], bytes_read);

                delay(100);

                Serial.println("Flash verify ...");
                if (!verify_flash(write_address, &buffer[4], MEGA_PAGE_SIZE))
                {
                    Serial.println("verify failed!");
                } 
                else 
                {
                    Serial.println("verify passed!");
                }
                write_address += MEGA_PAGE_SIZE;
                bytes_read = atmelFile.read(&buffer[4], MEGA_PAGE_SIZE);
            }



            atmelFile.close();
        }

        {
            uint8_t hash[24];
            String version = "";
            hash[0] = 0x02;
            hash[1] = 0x02;
            hash[2] = 0x00;
            hash[3] = 0x00;
            mbedtls_sha1_finish(&ctx, &hash[4]);
            Wire.beginTransmission(MEGA_BL_ADDRESS);
            if(Wire.write(hash, sizeof(hash)) != sizeof(hash))
            	Serial.println("Page not sent completely!");

            Wire.endTransmission();

            for (uint8_t i = 4; i < 24; i++)
            {
                version += String(hash[i], HEX);
            }
            Serial.println("Updated to version:\t" + version);
        }  


    	Serial.println("Update: Done");
    } else {
    	Serial.println("Version Match: No Update!");
    }
}


void Mega_Handler_Class::init()
{
    Bluetooth_Config bt_config;
    Preferences_Handler.getBluetoothConfig(bt_config);

    if (bt_config.enabled)
    {
        String fv = BP32.firmwareVersion();
        Serial.print("Bluepad32 Firmware: ");
        Serial.println(fv);

        // Setup the Bluepad32 callbacks
        BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    }

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

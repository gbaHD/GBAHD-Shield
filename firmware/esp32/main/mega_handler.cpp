
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
#include <LittleFS.h>
#include <mbedtls/sha1.h>
#include <uni_bluetooth.h>

#include "mega_handler.h"
#include "preferences_handler.h"
#include "log_handler.h"



#define MAP_BUTTON(INPUT, OUTPUT, I_MASK, O_MASK) (OUTPUT |= ( INPUT & I_MASK ) ? O_MASK : 0)

#define SDA_PIN         (32)
#define SCL_PIN         (33)

#define SHIELD_TIMEOUT  (5)

GamepadPtr Mega_Handler_Class::controller;


void Mega_Handler_Class::onConnectedGamepad(GamepadPtr gp) 
{
    if (!controller)
    {
        controller = gp;
        Log_Handler.println("CALLBACK: Controller is connected!");
    }
}

void Mega_Handler_Class::onDisconnectedGamepad(GamepadPtr gp) 
{
  Log_Handler.println("CALLBACK: Controller is disconnected!");
  controller = nullptr;
}

void Mega_Handler_Class::update_controller()
{
    static uint16_t previous_outputs = 0U;
    uint16_t new_outputs = 0U;
    Bluetooth_Config bt_config;
    Preferences_Handler.getBluetoothConfig(bt_config);

    if (bt_config.enabled)
    {
        BP32.update();

        if (controller && controller->isConnected()) 
        {
            MAP_BUTTON(((controller->axisY() < -80) ? DPAD_UP : 0), new_outputs, DPAD_UP, bt_config.mapping[BT_INP_UP]);
            MAP_BUTTON(((controller->axisY() > 80) ? DPAD_DOWN : 0), new_outputs, DPAD_DOWN, bt_config.mapping[BT_INP_DOWN]);
            MAP_BUTTON(((controller->axisX() < -80) ? DPAD_LEFT : 0), new_outputs, DPAD_LEFT, bt_config.mapping[BT_INP_LEFT]);
            MAP_BUTTON(((controller->axisX() > 80) ? DPAD_RIGHT : 0), new_outputs, DPAD_RIGHT, bt_config.mapping[BT_INP_RIGHT]);
            MAP_BUTTON(controller->dpad(), new_outputs, DPAD_UP,  bt_config.mapping[BT_INP_UP]);
            MAP_BUTTON(controller->dpad(), new_outputs, DPAD_DOWN, bt_config.mapping[BT_INP_DOWN]);
            MAP_BUTTON(controller->dpad(), new_outputs, DPAD_LEFT, bt_config.mapping[BT_INP_LEFT]);
            MAP_BUTTON(controller->dpad(), new_outputs, DPAD_RIGHT, bt_config.mapping[BT_INP_RIGHT]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_B, bt_config.mapping[BT_INP_B]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_A, bt_config.mapping[BT_INP_A]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_Y, bt_config.mapping[BT_INP_Y]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_X, bt_config.mapping[BT_INP_X]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_TRIGGER_L, bt_config.mapping[BT_INP_TR_L]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_SHOULDER_L, bt_config.mapping[BT_INP_SH_L]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_TRIGGER_R, bt_config.mapping[BT_INP_TR_R]);
            MAP_BUTTON(controller->buttons(), new_outputs, BUTTON_SHOULDER_R, bt_config.mapping[BT_INP_SH_R]);
            MAP_BUTTON(controller->miscButtons(), new_outputs, MISC_BUTTON_SYSTEM, bt_config.mapping[BT_INP_SYSTEM]);
            MAP_BUTTON(controller->miscButtons(), new_outputs, MISC_BUTTON_HOME, bt_config.mapping[BT_INP_START]);
            MAP_BUTTON(controller->miscButtons(), new_outputs, MISC_BUTTON_BACK, bt_config.mapping[BT_INP_SELECT]);

            new_outputs |= CTRL_BT_AVAILABLE;
        }
    }
    else
    {
        controller = nullptr;
    }

    if (new_outputs != previous_outputs)
    {
        Wire.beginTransmission(MEGA_BL_ADDRESS);
        Wire.flush();
        Wire.write(reinterpret_cast<uint8_t*>(&new_outputs), sizeof(uint16_t));
        Wire.endTransmission();
        Wire.flush();

        previous_outputs = new_outputs;
        Log_Handler.print("Current Controls: 0x");
        Log_Handler.println(new_outputs, HEX);
    }

}


String Mega_Handler_Class::string_hash(const uint8_t* const hash)
{
    String hash_string = "";
    for (uint8_t i = 0; i < 20; i++)
    {
        hash_string += String(hash[i], HEX);
    }

    return hash_string;
}

void Mega_Handler_Class::get_update_version(String& version)
{
    uint8_t hash[20] = {0};
    File atmelFile = LittleFS.open(ATMEGA_SPIFFS_PATH, "r");

    if (atmelFile)
    {
        mbedtls_sha1_context ctx;
        mbedtls_sha1_init(&ctx);
        mbedtls_sha1_starts(&ctx);
        uint8_t buffer[MEGA_PAGE_SIZE];

        size_t bytes_read = 0U;
        do
        {
            bytes_read = atmelFile.readBytes(reinterpret_cast<char*>(buffer), MEGA_PAGE_SIZE);
            mbedtls_sha1_update(&ctx, buffer, bytes_read);
        } while (bytes_read > 0U);
        mbedtls_sha1_finish(&ctx, hash);
    }

    atmelFile.close();

    version = string_hash(hash);
}

void Mega_Handler_Class::get_mega_version(String& version)
{
    uint8_t hash[20] = {0};
    uint8_t cmd[4] = {0x02, 0x02, 0x00, 0x00};

    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 20); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        Wire.readBytes(hash, 20);
    }

    version = string_hash(hash);
}


void Mega_Handler_Class::get_mega_hash(uint8_t hash[20])
{
    uint8_t cmd[4] = {0x02, 0x02, 0x00, 0x00};

    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(cmd, sizeof(cmd));   // <-- Command Read
    Wire.endTransmission(false);
    Wire.requestFrom(MEGA_BL_ADDRESS, 20); // <-- Request only 7 Byte for short commit hash

    while (Wire.available()) {
        Wire.readBytes(hash, 20);
    }
}

void Mega_Handler_Class::get_update_hash(uint8_t hash[20])
{
    File atmelFile = LittleFS.open(ATMEGA_SPIFFS_PATH, "r");

    if (atmelFile)
    {
        mbedtls_sha1_context ctx;
        mbedtls_sha1_init(&ctx);
        mbedtls_sha1_starts(&ctx);
        uint8_t buffer[MEGA_PAGE_SIZE];

        size_t bytes_read = 0U;
        do
        {
            bytes_read = atmelFile.readBytes(reinterpret_cast<char*>(buffer), MEGA_PAGE_SIZE);
            mbedtls_sha1_update(&ctx, buffer, bytes_read);
        } while (bytes_read > 0U);
        mbedtls_sha1_finish(&ctx, hash);
    }

    atmelFile.close();
}

void Mega_Handler_Class::restart_shield()
{
    uint16_t restart = RESET_SHIELD;
    Log_Handler.println("Restarting shield");
    Wire.flush();
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(reinterpret_cast<uint8_t*>(&restart), sizeof(uint16_t));
    Wire.endTransmission();
    delay(100);
    stop_bootloader();
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
	Log_Handler.println("Starting application");
    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(0x01);
    Wire.write(0x80);
    Wire.endTransmission();
    Wire.flush();
}

bool Mega_Handler_Class::verify_page(uint16_t address, const uint8_t page[MEGA_PAGE_SIZE])
{
    uint8_t cmd[4] = {0x02, 0x01, highByte(address), lowByte(address)};
    uint16_t timeout = 0U;
    bool ret = true;

    Log_Handler.println("Flash verify ...");
    Log_Handler.print("Waiting for TWI response");

    do {
        Log_Handler.print(".");
        
        Wire.beginTransmission(MEGA_BL_ADDRESS);
        Wire.write(cmd, sizeof(cmd)/sizeof(uint8_t));   // <-- Command Read
        Wire.endTransmission(false);
        Wire.requestFrom(MEGA_BL_ADDRESS, MEGA_PAGE_SIZE);

        delay(100);
        if (++timeout > MEGA_RESPONSE_TIMEOUT)
        {
            ret = false;
            Log_Handler.println("Timeout!");
        }
    } while ((!Wire.available()) && (timeout < MEGA_RESPONSE_TIMEOUT));

    for(uint16_t i = 0U; (i < MEGA_PAGE_SIZE) && ret; i++)
    {
        if (page[i] != Wire.read())
        {
            ret = false;
        }
    }

    if (!ret)
    {
        Log_Handler.println("verify failed!");
    } 
    else 
    {
        Log_Handler.println("verify passed!");
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
    if (Wire.available())
    {
        Log_Handler.print("Chip Info: ");
        for (uint8_t i = 0U; i < 8; i++)
        {
            Log_Handler.print(i);
            Log_Handler.print(" - ");
            Log_Handler.println(static_cast<char>(Wire.read()), HEX);
        }
        Log_Handler.println("");
        ret = true;
    }
    else
    {
        Log_Handler.println("TWI is not answering...");
        ret = false;
    }
    return ret;
}

bool Mega_Handler_Class::flash_page(uint16_t address, const uint8_t page[MEGA_PAGE_SIZE])
{
    uint8_t cmd[4] = {0x02, 0x01, highByte(address), lowByte(address)};
    bool ret = true;

    Wire.beginTransmission(MEGA_BL_ADDRESS);
    Wire.write(cmd, 4);

    if(Wire.write(page, MEGA_PAGE_SIZE) != MEGA_PAGE_SIZE)
    {
        Log_Handler.println("Page not sent completely!");
        ret = false;
    }

    Wire.endTransmission();

    return ret;
}

void Mega_Handler_Class::update_mega(bool force)
{
    uint8_t oldHash[20] = {0}, newHash[20] = {0};
    uint8_t shield_timeout = 0U;
    bool shield_available = get_chip_info();


    while ((!shield_available) && (shield_timeout < SHIELD_TIMEOUT))
    {
        Log_Handler.println("Press Reset on Shield... Retrying - " + String(shield_timeout));
        restart_shield();
        delay(500);
        shield_timeout++;
        shield_available = get_chip_info();
    }
    
    if (shield_available)
    {
        get_mega_hash(oldHash);
        get_update_hash(newHash);

        if (memcmp(oldHash, newHash, 20) || force)
        {
            bool update_success = true;
            {
                File atmelFile = LittleFS.open(ATMEGA_SPIFFS_PATH, "r");
                uint16_t write_address = 0;
                uint8_t bytes_read = 0U;
                uint8_t buffer[MEGA_PAGE_SIZE] = {0};

                Log_Handler.println("Updating Shield...");

                bytes_read = atmelFile.read(buffer, MEGA_PAGE_SIZE);

                while ((bytes_read > 0) && (update_success))
                {
                    Log_Handler.print("Writing at ");
                    Log_Handler.println(write_address, HEX);
                    update_success = flash_page(write_address, buffer);
                    update_success &= verify_page(write_address, buffer);
                    memset(buffer, 0xFF, MEGA_PAGE_SIZE);

                    write_address += MEGA_PAGE_SIZE;
                    bytes_read = atmelFile.read(buffer, MEGA_PAGE_SIZE);
                }

                atmelFile.close();
            }
            if(update_success)
            {
                uint8_t cmd[4] = {0x02, 0x02, 0x00, 0x00};

                Wire.beginTransmission(MEGA_BL_ADDRESS);
                Wire.write(cmd, sizeof(cmd));
                Wire.write(newHash, sizeof(newHash));
                Wire.endTransmission();

                Log_Handler.print("Updated to version:\t" + string_hash(newHash));
            }  
            else
            {
                Log_Handler.println("Update: failed");
            }
            
        } 
        else 
        {
            Log_Handler.println("Version Match: No Update!");
        }
    }
    else
    {
        Log_Handler.println("Shield is not available. Update failed.");
    }
}

void Mega_Handler_Class::trigger_external_update(bool force)
{
    restart_shield();
    update_mega(force);
    start_application();
}


void Mega_Handler_Class::init()
{
    Bluetooth_Config bt_config;
    Preferences_Handler.getBluetoothConfig(bt_config);

    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    uni_bluetooth_enable_new_connections_safe(bt_config.enabled);
    

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

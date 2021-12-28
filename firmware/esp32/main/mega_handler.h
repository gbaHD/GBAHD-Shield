#ifndef _MEGA_HANDLER_H
#define _MEGA_HANDLER_H

#include <Bluepad32.h>


class Mega_Handler_Class 
{
    public:
        void init();
        void update();
        void get_mega_version(String& version);
        void get_update_version(String& version);
    private:
        void update_controller();
        void update_mega();
        void stop_bootloader();
        void start_application();
        void print_chip_info();
        bool verify_flash(uint16_t address, const uint8_t* buffer, size_t size);

        uint16_t get_twi_flash_bytes(uint16_t address, uint8_t* buffer, size_t size);

        static void onConnectedGamepad(GamepadPtr gp);
        static void onDisconnectedGamepad(GamepadPtr gp);
        static GamepadPtr controller;
};

extern Mega_Handler_Class Mega_Handler;

#endif
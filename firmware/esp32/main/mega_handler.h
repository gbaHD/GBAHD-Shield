#ifndef _MEGA_HANDLER_H
#define _MEGA_HANDLER_H

#include <Bluepad32.h>


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
#define RESET_SHIELD   (CTRL_IN_LEFT | CTRL_IN_RIGHT | CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT | CTRL_IN_A | CTRL_IN_X)


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
        void restart_shield();
        void start_application();
        bool get_chip_info();
        bool verify_flash(uint16_t address, const uint8_t* buffer, size_t size);

        uint16_t get_twi_flash_bytes(uint16_t address, uint8_t* buffer, size_t size);

        static void onConnectedGamepad(GamepadPtr gp);
        static void onDisconnectedGamepad(GamepadPtr gp);
        static GamepadPtr controller;
};

extern Mega_Handler_Class Mega_Handler;

#endif
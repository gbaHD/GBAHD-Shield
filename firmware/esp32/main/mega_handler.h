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

#ifndef _MEGA_HANDLER_H
#define _MEGA_HANDLER_H

#include <Bluepad32.h>


#define CTRL_IN_A         (0x0008)
#define CTRL_IN_B         (0x0800)
#define CTRL_IN_Y         (0x0400)
#define CTRL_IN_X         (0x0004)
#define CTRL_IN_UP        (0x0080)
#define CTRL_IN_DOWN      (0x0040)
#define CTRL_IN_LEFT      (0x0020)
#define CTRL_IN_RIGHT     (0x0010)
#define CTRL_IN_L         (0x0002)
#define CTRL_IN_R         (0x0001)
#define CTRL_IN_START     (0x0100)
#define CTRL_IN_SELECT    (0x0200)
#define CTRL_BT_AVAILABLE (0x8000)

#define ENABLE_OSD     (CTRL_IN_UP | CTRL_IN_R | CTRL_IN_L | CTRL_IN_SELECT)
#define RESET_SHIELD   (CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT | CTRL_IN_A | CTRL_IN_Y | CTRL_IN_LEFT | CTRL_IN_RIGHT | CTRL_BT_AVAILABLE)
#define ATMEGA_SPIFFS_PATH ( "/atmega.bin" )

#define MEGA_BL_ADDRESS             (0x29)
#define MEGA_PAGE_SIZE              (0x80)  // <-- Page Size of ATMega328P
#define MEGA_RESPONSE_TIMEOUT       (10)


class Mega_Handler_Class 
{
    public:
        void init();
        void update();
        void get_mega_version(String& version);
        void get_update_version(String& version);
        void get_mega_hash(uint8_t hash[20]);
        void get_update_hash(uint8_t hash[20]);
        void trigger_external_update(bool force);
        void get_controller_name(String& name);
        bool get_shield_available(void) {return shield_available;};
    private:
        void update_mega(bool force = false);
        void update_controller();
        void stop_bootloader();
        void restart_shield();
        void start_application();
        bool get_chip_info();
        String string_hash(const uint8_t* const hash);
        bool verify_page(uint16_t address, const uint8_t page[MEGA_PAGE_SIZE]);
        bool flash_page(uint16_t address, const uint8_t page[MEGA_PAGE_SIZE]);

        static void onConnectedGamepad(GamepadPtr gp);
        static void onDisconnectedGamepad(GamepadPtr gp);
        static GamepadPtr controller;
        bool shield_available = false;
};

extern Mega_Handler_Class Mega_Handler;

#endif

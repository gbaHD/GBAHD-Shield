/*******************************************************************************
  ; -----------------------------------------------------------------------
   gbaHD-Shield for zwenergys gbaHD

   MIT License

   Copyright (c) 2021 - 2022 Christian Ruecker

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

 ********************************************************************************

   This program is designed to run on a AVR ATmega168p / ATmega328p microcontroller connected
   to a SNES controller, the Spartan Edge Accelerator Board (SEAB) and a GBA mainboard (GBA)

   pin configuration: (external pin)

   ATmega168p/ATmega328p
                                     ,-----_-----.
                              !RESET |1  C6 C5 28| SCL
                                  TX |2  D0 C4 27| SDA
                                  RX |3  D1 C3 26| Testpin T4
                    GBA_POWER_ENABLE |4  D2 C2 25| Testpin T3
                           FPGA_COMM |5  D3 C1 24| Testpin T2
                          Testpin T5 |6  D4 C0 23| Testpin T1
                                 VCC |7        22| GND
                                 GND |8        21| VCC
                                  NC |9  B6    20| VCC
                                  NC |10 B7 B5 19| serial clock
                          Testpin T6 |11 D5 B4 18| serial data in (read controller data)
                             LED Red |12 D6 B3 17| serial data out (write GBA controls)
                           LED Green |13 D7 B2 16| serial chip select/latch
                          Testpin T7 |14 B0 B1 15| GBA_OUTPUT_EN
                                     `-----------'

 ********************************************************************************

   ToDo:
        - Button press simulation (generating buttons press by the microcontroller)

 *******************************************************************************/
/* === INCLUDES ============================================================= */
#include <global.h>

#include "debug.h"

#include "controller.h"
#include "osd.h"
#include "twiSlave.h"
/* === TYPES ================================================================ */

/* === MACROS =============================================================== */

/* === GLOBALS ============================================================== */

bool osd_enabled = false;
volatile uint32_t twi_data_valid_ms = 0;
uint8_t shield_variant = MANCLOUD;

/* === PROTOTYPES =========================================================== */
void process_data(uint16_t ctrl_data);
void reboot(void);
void get_shield_variant();

int main(void) {
    init_uart();
    printf("RstRsn: %02X\n", MCUSR);

    MCUSR = 0;
    wdt_disable();

    get_shield_variant();
    if(shield_variant == MANCLOUD) {
        MC_LED_DIR |= _BV(MC_LED_GREEN_PIN) | _BV(MC_LED_RED_PIN);
        MC_FPGA_COMM_PORT |= _BV(MC_FPGA_COMM_PIN);
        MC_FPGA_COMM_DIR |= _BV(MC_FPGA_COMM_PIN);
    } else if (shield_variant == CONSOLES4YOU) {
        C4Y_SETPIN_M(C4Y_RESET_DIR, C4Y_RESET_PIN, 1);
        C4Y_LED_DIR |= _BV(C4Y_LED_RED_PIN);
        C4Y_LED_PORT |= _BV(C4Y_LED_RED_PIN);
        C4Y_FPGA_COMM_PORT |= _BV(C4Y_FPGA_COMM_PIN);
        C4Y_FPGA_COMM_DIR |= _BV(C4Y_FPGA_COMM_PIN);
    }
    controller_init();
    twi_init();
    reboot();
    osd_init();
    while(1) {
        controller_update();
        printf("controller_data: %04X \n", get_controller_data());
        if(twi_available()){
            uint16_t tmpVal = twi_read();
            if(tmpVal & CTRL_BT_CONNECTED) {
                set_controller_data(tmpVal & CTRL_DATA_MASK);
            } else {
                twi_flush();
            }
        }
        process_data(get_controller_data());
        controller_map_data();
    }
}

void process_data(uint16_t ctrl_data) {
      static uint16_t last_combination = 0xFFF;

      if (osd_enabled) {
          osd_enabled = osd_update(ctrl_data);
        set_controller_data(0);    //clear controller data as long as OSD is enabled
      } else if(pad_display_enabled()) {
          pad_display_update(ctrl_data);
      }

      if (last_combination != ctrl_data) {
          last_combination = ctrl_data;
          switch (ctrl_data)
          {
              case COMBO_IGR: {
                  reboot();
              } break;
              case ENABLE_OSD: {
                  osd_enter();
                  osd_enabled = true;
              } break;
              case COMBO_AVR_RST: {
                  wdt_enable(WDTO_120MS);
                  while(1);
              } break;
          }
      }
}

void reboot(void) {
    if(shield_variant == MANCLOUD) {
        MC_GBA_OUTPUT_EN_M(false);
        MC_GBA_POWER_OFF_M();
        _delay_ms(500);
        MC_GBA_POWER_ON_M();
    } else if (shield_variant == CONSOLES4YOU) {
        C4Y_SETPIN_M(C4Y_RESET_PORT, C4Y_RESET_PIN, 0);
        _delay_ms(1000);
        C4Y_SETPIN_M(C4Y_RESET_PORT, C4Y_RESET_PIN, 1);
    }
}

void get_shield_variant(void){
    if(boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS) == 0xFF) {
        shield_variant = CONSOLES4YOU;
    }
}

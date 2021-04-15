/*******************************************************************************
; -----------------------------------------------------------------------
 * gbaHD-Shield for zwenergys gbaHD
 *
 * MIT License
 *
 * Copyright (c) 2021 Christian Ruecker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * This program is designed to run on a AVR ATmega168p / ATmega328p microcontroller connected
 * to a SNES controller, the Spartan Edge Accelerator Board (SEAB) and a GBA mainboard (GBA)
 *
 * pin configuration: (external pin)
 *
 * ATmega168p/ATmega328p
 *                                   ,-----_-----.
 *                            !RESET |1  C6 C5 28| NC
 *                                TX |2  D0 C4 27| Testpin T4
 *                                RX |3  D1 C3 26| Testpin T3
 *                  GBA_POWER_SELECT |4  D2 C2 25| Testpin T2
 *                        PIXEL_GRID |5  D3 C1 24| Testpin T1
 *                                NC |6  D4 C0 23| NC
 *                               VCC |7        22| GND                 
 *                               GND |8        21| VCC
 *                                NC |9  B6    20| VCC
 *                                NC |10 B7 B5 19| serial clock                                     
 *                        Testpin T5 |11 D5 B4 18| serial data in (read controller data)
 *                        Testpin T6 |12 D6 B3 17| serial data out (write GBA controls)
 *                        Testpin T7 |13 D7 B2 16| serial chip select/latch
 *                        Testpin T8 |14 B0 B1 15| GBA_OUTPUT_EN
 *                                   `-----------'
 *                                   
 ********************************************************************************
 *
 * ToDo:
 *      - external Controller feed (via UART)
 *      - Button press simulation (generating buttons press by the microcontroller)
 *
 *******************************************************************************/
/* === INCLUDES ============================================================= */
#include "fw.h"

/* === TYPES ================================================================ */

/* === MACROS =============================================================== */

/* === GLOBALS ============================================================== */
uint16_t controller_data = 0xFF;

/* === PROTOTYPES =========================================================== */
uint16_t map_data(uint16_t ctrl_data);
void process_combination(uint16_t ctrl_data);
void reboot(void);
void toggle_grid(void);

/* === IMPLEMENTATION ======================================================= */
void setup(void) { 
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.begin();
  pinMode(SNES_LATCH, OUTPUT);
  digitalWrite(SNES_LATCH, HIGH);
  pinMode(GBA_OUTPUT_EN, OUTPUT);
  digitalWrite(GBA_OUTPUT_EN, HIGH);
  pinMode(GBA_POWER_SEL, OUTPUT);
  digitalWrite(GBA_POWER_SEL, HIGH);
  pinMode(PIXEL_GRID, OUTPUT); 
  
#if DEBUG == 1  
  Serial.begin(9600);
#endif
  delay(1000);
  reboot();
  }

void loop(void) {
  //The SNES Controller needs a falling edge on LATCH to know when to capture the data
  if(((controller_data) & 0x3FF) == 0) digitalWrite(GBA_OUTPUT_EN, HIGH);
  else digitalWrite(GBA_OUTPUT_EN, LOW);
  digitalWrite(SNES_LATCH, LOW);
  uint16_t in = (~SPI.transfer16((~controller_data) & 0x3FF))>>4;
  digitalWrite(SNES_LATCH, HIGH);
  
  controller_data = map_data(in);
  process_combination(in);         //process combination based on SNES input to also have X and Y for use
  
#if DEBUG == 1
  Serial.print(" Got value: ");
  Serial.println(~in & 0xFFF, HEX);
  delay(16); //<-- approx 60 Hz
#endif
}

uint16_t map_data(uint16_t ctrl_data){
  uint16_t out_data = 0;
    if (ctrl_data & CTRL_IN_A)      out_data |= GBA_OUT_A;
    if (ctrl_data & CTRL_IN_B)      out_data |= GBA_OUT_B;
    if (ctrl_data & CTRL_IN_X)      out_data |= GBA_OUT_B;
    if (ctrl_data & CTRL_IN_Y)      out_data |= GBA_OUT_A;
    if (ctrl_data & CTRL_IN_UP)     out_data |= GBA_OUT_UP;
    if (ctrl_data & CTRL_IN_DOWN)   out_data |= GBA_OUT_DOWN;
    if (ctrl_data & CTRL_IN_LEFT)   out_data |= GBA_OUT_LEFT;
    if (ctrl_data & CTRL_IN_RIGHT)  out_data |= GBA_OUT_RIGHT;
    if (ctrl_data & CTRL_IN_L)      out_data |= GBA_OUT_L;
    if (ctrl_data & CTRL_IN_R)      out_data |= GBA_OUT_R;
    if (ctrl_data & CTRL_IN_START)  out_data |= GBA_OUT_START;
    if (ctrl_data & CTRL_IN_SELECT) out_data |= GBA_OUT_SELECT;
    return out_data;
}

void process_combination(uint16_t ctrl_data){
  static uint16_t last_combination=0xFFF;
  if(last_combination != ctrl_data) {
    last_combination = ctrl_data;
    switch(ctrl_data)
    {
      case COMBO_IGR: {
        reboot(); 
      }break;
      case COMBO_PIXEL_GRID_TOGGLE: {
        toggle_grid();
      }break;
    }
  }
}

void reboot(void){
  digitalWrite(GBA_OUTPUT_EN, HIGH);
  delay(250);
  digitalWrite(GBA_POWER_SEL, HIGH);
  delay(500);
  digitalWrite(GBA_POWER_SEL, LOW);
  delay(250);
}

void toggle_grid(void){
  digitalWrite(PIXEL_GRID, HIGH);
  delay(10);
  digitalWrite(PIXEL_GRID, LOW);
}

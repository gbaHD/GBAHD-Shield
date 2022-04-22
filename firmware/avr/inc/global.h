/*
 * pins.h
 *
 *  Created on: 02.01.2022
 *      Author: ManCloud
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

/* === INCLUDES ============================================================= */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/boot.h>

/* === TYPES ================================================================ */
typedef enum {
    MANCLOUD,
    CONSOLES4YOU
}shield_type_enum;

/* === MACROS =============================================================== */
/* ManCloud gbaHD-Shield defines*/
#define MC_GBA_OUTPUT_EN_DIR       DDRB
#define MC_GBA_OUTPUT_EN_PORT      PORTB
#define MC_GBA_OUTPUT_EN_PIN       PIN1
#define MC_GBA_OUTPUT_EN_INIT_M()  DDRB |= _BV(MC_GBA_OUTPUT_EN_PIN)
#define MC_GBA_OUTPUT_EN_M(enable) MC_GBA_OUTPUT_EN_PORT = ((MC_GBA_OUTPUT_EN_PORT & ~_BV(MC_GBA_OUTPUT_EN_PIN)) | ((enable) ? 0 : _BV(MC_GBA_OUTPUT_EN_PIN)))

#define MC_CTRL_DIR        DDRB
#define MC_CTRL_PORT       PORTB
#define MC_CTRL_LATCH      PIN2
#define MC_GBA_DATA_OUT    PIN3
#define MC_CTRL_DATA_IN    PIN4
#define MC_CTRL_CLOCK      PIN5
#define MC_CTRL_INIT_M()   MC_CTRL_DIR |= _BV(MC_CTRL_LATCH) | _BV(MC_GBA_DATA_OUT) | _BV(MC_CTRL_CLOCK)

#define MC_LED_DIR         DDRD
#define MC_LED_PORT        PORTD
#define MC_LED_RED_PIN     PIN6
#define MC_LED_GREEN_PIN   PIN7

#define MC_GBA_POWER_ON_DIR    DDRD
#define MC_GBA_POWER_ON_PIN    PIN2
#define MC_GBA_POWER_ON_M()    MC_GBA_POWER_ON_DIR |= _BV(MC_GBA_POWER_ON_PIN)
#define MC_GBA_POWER_OFF_M()   MC_GBA_POWER_ON_DIR &= ~_BV(MC_GBA_POWER_ON_PIN)

#define MC_FPGA_COMM_DIR   DDRD
#define MC_FPGA_COMM_PORT  PORTD
#define MC_FPGA_COMM_PIN   PIN3

/* Consoles4You gbaHD-HAT defines */
#define C4Y_CTRL_DIR        DDRC
#define C4Y_CTRL_PORT       PORTC
#define C4Y_CTRL_INPUT      PINC
#define C4Y_CTRL_LATCH      PIN1
#define C4Y_CTRL_DATA_IN    PIN2
#define C4Y_CTRL_CLOCK      PIN0
#define C4Y_CTRL_INIT_M()   C4Y_CTRL_DIR |= _BV(C4Y_CTRL_LATCH) | _BV(C4Y_CTRL_CLOCK)

#define C4Y_FPGA_COMM_DIR   DDRC
#define C4Y_FPGA_COMM_PORT  PORTC
#define C4Y_FPGA_COMM_PIN   PIN3

#define C4Y_LED_DIR         DDRB
#define C4Y_LED_PORT        PORTB
#define C4Y_LED_RED_PIN     PIN5
#define C4Y_LED_GREEN_PIN   10      //undefined

#define C4Y_OUT_DIR_UP DDRD
#define C4Y_OUT_PIN_UP PIN3

#define C4Y_OUT_DIR_DOWN DDRD
#define C4Y_OUT_PIN_DOWN PIN5

#define C4Y_OUT_DIR_LEFT DDRD
#define C4Y_OUT_PIN_LEFT PIN4

#define C4Y_OUT_DIR_RIGHT DDRD
#define C4Y_OUT_PIN_RIGHT PIN6

#define C4Y_OUT_DIR_START DDRB
#define C4Y_OUT_PIN_START PIN0

#define C4Y_OUT_DIR_SELECT DDRD
#define C4Y_OUT_PIN_SELECT PIN7

#define C4Y_OUT_DIR_A DDRB
#define C4Y_OUT_PIN_A PIN2

#define C4Y_OUT_DIR_B DDRB
#define C4Y_OUT_PIN_B PIN3

#define C4Y_OUT_DIR_L DDRD
#define C4Y_OUT_PIN_L PIN2

#define C4Y_OUT_DIR_R DDRB
#define C4Y_OUT_PIN_R PIN1

#define C4Y_SETPIN_M(reg, pin, state) {reg = ((reg & ~_BV(pin)) | (state ? _BV(pin) : 0));}

#define C4Y_RESET_PORT PORTB
#define C4Y_RESET_DIR  PORTB
#define C4Y_RESET_PIN  PIN4
/* === GLOBALS ============================================================== */
extern uint8_t shield_variant;

/* === PROTOTYPES =========================================================== */

#endif //GLOBAL_H_

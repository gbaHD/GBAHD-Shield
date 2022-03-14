/*
 * pins.h
 *
 *  Created on: 02.01.2022
 *      Author: ManCloud
 */

#ifndef PINS_H_
#define PINS_H_

/* === INCLUDES ============================================================= */

/* === TYPES ================================================================ */

/* === MACROS =============================================================== */
#define GBA_OUTPUT_EN_DIR       DDRB
#define GBA_OUTPUT_EN_PORT      PORTB
#define GBA_OUTPUT_EN_PIN       PIN1
#define GBA_OUTPUT_EN_INIT_M()  DDRB |= _BV(GBA_OUTPUT_EN_PIN)
#define GBA_OUTPUT_EN_M(enable) GBA_OUTPUT_EN_PORT = ((GBA_OUTPUT_EN_PORT & ~_BV(GBA_OUTPUT_EN_PIN)) | ((enable) ? 0 : _BV(GBA_OUTPUT_EN_PIN)))

#define CTRL_DIR        DDRB
#define CTRL_PORT       PORTB
#define CTRL_LATCH      PIN2
#define GBA_DATA_OUT    PIN3
#define CTRL_DATA_IN    PIN4
#define CTRL_CLOCK      PIN5
#define CTRL_INIT_M()   CTRL_DIR |= _BV(CTRL_LATCH) | _BV(GBA_DATA_OUT) | _BV(CTRL_CLOCK)

#define LED_DIR         DDRD
#define LED_PORT        PORTD
#define LED_RED_PIN     PIN6
#define LED_GREEN_PIN   PIN7

#define GBA_POWER_ON_DIR    DDRD
#define GBA_POWER_ON_PIN    PIN2
#define GBA_POWER_ON_M()    GBA_POWER_ON_DIR |= _BV(GBA_POWER_ON_PIN)
#define GBA_POWER_OFF_M()   GBA_POWER_ON_DIR &= ~_BV(GBA_POWER_ON_PIN)

#define FPGA_COMM_DIR   DDRD
#define FPGA_COMM_PORT  PORTD
#define FPGA_COMM_PIN   PIN3

/* === GLOBALS ============================================================== */

/* === PROTOTYPES =========================================================== */

#endif //FW_H_

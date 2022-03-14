/*
 * controller.c
 *
 *  Created on: 02.01.2022
 *      Author: ManCloud
 */

/* === INCLUDES ============================================================= */
#include "controller.h"

/* === TYPES ================================================================ */

/* === MACROS =============================================================== */

/* === GLOBALS ============================================================== */
uint16_t controller_data = 0x0000;
uint16_t gba_data = 0x0000;

/* === PROTOTYPES =========================================================== */
uint8_t transfer_byte(uint8_t data);
uint16_t transfer_16bit(uint16_t data);

/* === IMPLEMENTATION ======================================================= */
uint16_t get_controller_data(void) {
    return controller_data;
}

void set_controller_data(uint16_t data) {
    controller_data = data & CTRL_DATA_MASK;
}

void controller_init(void) {
    CTRL_INIT_M();
    GBA_OUTPUT_EN_INIT_M();

    //SPI Mode 2 @ fosc/32
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1); //| _BV(CPOL);
    SPSR |= _BV(SPI2X);
}

void controller_update(void) {
    GBA_OUTPUT_EN_M(gba_data != 0);
    CTRL_PORT &= ~_BV(CTRL_LATCH);
//    _delay_ms(1);
    controller_data = ((~transfer_16bit(~(gba_data & GBA_DATA_MASK))));
    controller_data = (controller_data == 0xFFFF) ? 0 : ((controller_data >> 4) & CTRL_DATA_MASK);
    CTRL_PORT |= _BV(CTRL_LATCH);
}

void controller_map_data(void) {
    gba_data = 0;
    if (controller_data & CTRL_IN_A)      gba_data |= GBA_OUT_A;
    if (controller_data & CTRL_IN_B)      gba_data |= GBA_OUT_B;
    if (controller_data & CTRL_IN_X)      gba_data |= GBA_OUT_B;
    if (controller_data & CTRL_IN_Y)      gba_data |= GBA_OUT_A;
    if (controller_data & CTRL_IN_UP)     gba_data |= GBA_OUT_UP;
    if (controller_data & CTRL_IN_DOWN)   gba_data |= GBA_OUT_DOWN;
    if (controller_data & CTRL_IN_LEFT)   gba_data |= GBA_OUT_LEFT;
    if (controller_data & CTRL_IN_RIGHT)  gba_data |= GBA_OUT_RIGHT;
    if (controller_data & CTRL_IN_L)      gba_data |= GBA_OUT_L;
    if (controller_data & CTRL_IN_R)      gba_data |= GBA_OUT_R;
    if (controller_data & CTRL_IN_START)  gba_data |= GBA_OUT_START;
    if (controller_data & CTRL_IN_SELECT) gba_data |= GBA_OUT_SELECT;
}

uint8_t transfer_byte(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));
    return SPDR;
}

uint16_t transfer_16bit(uint16_t data) {
    uint16_t retVal;
    retVal = transfer_byte(data >> 8) << 8;
    retVal |= transfer_byte(data & 0xFF);
    return retVal;
}

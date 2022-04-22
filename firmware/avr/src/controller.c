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
volatile uint16_t controller_data = 0x0000;
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
    if(shield_variant == MANCLOUD) {
        MC_CTRL_INIT_M();
        MC_GBA_OUTPUT_EN_INIT_M();

        //SPI Mode 2 @ fosc/32
        SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1); //| _BV(CPOL);
        SPSR |= _BV(SPI2X);
    } else if (shield_variant == CONSOLES4YOU) {
        C4Y_CTRL_INIT_M();
        C4Y_SETPIN_M(C4Y_OUT_PORT_UP,       C4Y_OUT_PIN_UP,     0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_DOWN,     C4Y_OUT_PIN_DOWN,   0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_LEFT,     C4Y_OUT_PIN_LEFT,   0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_RIGHT,    C4Y_OUT_PIN_RIGHT,  0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_START,    C4Y_OUT_PIN_START,  0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_SELECT,   C4Y_OUT_PIN_SELECT, 0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_A,        C4Y_OUT_PIN_A,      0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_B,        C4Y_OUT_PIN_B,      0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_L,        C4Y_OUT_PIN_L,      0);
        C4Y_SETPIN_M(C4Y_OUT_PORT_R,        C4Y_OUT_PIN_R,      0);
    }
}

void controller_update(void) {
    if(shield_variant == MANCLOUD) {
        MC_GBA_OUTPUT_EN_M(gba_data != 0);
        MC_CTRL_PORT &= ~_BV(MC_CTRL_LATCH);
        //    _delay_ms(1);
        controller_data = ((~transfer_16bit(~(gba_data & GBA_DATA_MASK))));
        MC_CTRL_PORT |= _BV(MC_CTRL_LATCH);
    } else if (shield_variant == CONSOLES4YOU) {
        // Output GBA data - START
        C4Y_SETPIN_M(C4Y_OUT_DIR_UP,        (C4Y_OUT_PIN_UP),       (gba_data & GBA_OUT_UP)     );
        C4Y_SETPIN_M(C4Y_OUT_DIR_DOWN,      (C4Y_OUT_PIN_DOWN),     (gba_data & GBA_OUT_DOWN)   );
        C4Y_SETPIN_M(C4Y_OUT_DIR_LEFT,      (C4Y_OUT_PIN_LEFT),     (gba_data & GBA_OUT_LEFT)   );
        C4Y_SETPIN_M(C4Y_OUT_DIR_RIGHT,     (C4Y_OUT_PIN_RIGHT),    (gba_data & GBA_OUT_RIGHT)  );
        C4Y_SETPIN_M(C4Y_OUT_DIR_START,     (C4Y_OUT_PIN_START),    (gba_data & GBA_OUT_START)  );
        C4Y_SETPIN_M(C4Y_OUT_DIR_SELECT,    (C4Y_OUT_PIN_SELECT),   (gba_data & GBA_OUT_SELECT) );
        C4Y_SETPIN_M(C4Y_OUT_DIR_A,         (C4Y_OUT_PIN_A),        (gba_data & GBA_OUT_A)      );
        C4Y_SETPIN_M(C4Y_OUT_DIR_B,         (C4Y_OUT_PIN_B),        (gba_data & GBA_OUT_B)      );
        C4Y_SETPIN_M(C4Y_OUT_DIR_L,         (C4Y_OUT_PIN_L),        (gba_data & GBA_OUT_L)      );
        C4Y_SETPIN_M(C4Y_OUT_DIR_R,         (C4Y_OUT_PIN_R),        (gba_data & GBA_OUT_R)      );
        // Output GBA data - END

        // Fetch controller data - START
        controller_data = 0;
        C4Y_CTRL_PORT &= ~_BV(C4Y_CTRL_LATCH);
        for(uint8_t i = 0; i < 16; i++) {
            C4Y_CTRL_PORT &= ~_BV(C4Y_CTRL_CLOCK);
            _delay_us(12);
            controller_data <<= 1;
            if(i < 12) {
                controller_data |= ((C4Y_CTRL_INPUT & _BV(C4Y_CTRL_DATA_IN)) ? 0x0001 : 0);
            } else {
                controller_data |= 1;
            }
            C4Y_CTRL_PORT |= _BV(C4Y_CTRL_CLOCK);
            _delay_us(12);
        }
        C4Y_CTRL_PORT |= _BV(C4Y_CTRL_LATCH);
        controller_data = ~controller_data;
        // Fetch controller data - END
    }
    controller_data = (controller_data == 0xFFFF) ? 0 : ((controller_data >> 4) & CTRL_DATA_MASK);
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

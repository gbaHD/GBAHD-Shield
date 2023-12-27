/*
 * osd.c
 *
 *  Created on: 13.01.2023
 *      Author: ManCloud
 */


#include "osd.h"

/* Current OSD:
 * 0. /----------------\
 * 1. |  GBAHD V1.3K   |
 * 2. \----------------/
 * 3. PXL GRID      ON|OFF
 * 4. METHOD        ADD|MULT
 * 5. SMOOTHING     OFF|2x|4x
 * 6. COLOR         NORMAL|GBA
 * 7. FRAMERATE     59.7HZ|60HZ
 * 8. PAD DISP      OFF|ON
 * */

void fpga_transfer(uint16_t data);
void fpga_write_bit(bool high);

config_t config_byte = {0,0,0,0,0,0};
config_t config_byte_bak = {0,0,0,0,0,0};

void osd_write_config(void) {
        fpga_transfer(0x4000 | *((uint8_t*)(&config_byte)));
}

void osd_init(void){
    uint8_t tmpVal = eeprom_read_byte((uint8_t *)(E2END-1));
    if(tmpVal == 0xFF) tmpVal = 0;
    config_byte = *((config_t *)(&tmpVal));
    osd_write_config();
}

void osd_enter(void) {
    config_byte_bak = config_byte;
    if(config_byte.padDisp) {
        _delay_ms(10);
        fpga_transfer( (0x8000));
        _delay_ms(10);
        fpga_transfer( (0x8000));
        _delay_ms(10);
    }
}


#define OSD_MIN_IDX 3
#define OSD_MAX_IDX 8
bool osd_update(uint16_t buttons) {
    bool retVal = true;

    static uint8_t osd_idx = OSD_MIN_IDX;
    static uint16_t last_buttons = 0;

    if(buttons != last_buttons) {
        last_buttons = buttons;
        if(buttons) {
            bool update_osd = true;
            switch(buttons){
                case CTRL_IN_UP: {
                    osd_idx = ((osd_idx > OSD_MIN_IDX) ? (osd_idx - 1) : OSD_MIN_IDX);
                } break;
                case CTRL_IN_DOWN: {
                    osd_idx = ((osd_idx < OSD_MAX_IDX) ? (osd_idx + 1) : OSD_MAX_IDX);
                } break;
                case CTRL_IN_A: {
                    update_osd = false;
                    switch (osd_idx){
                        case 3: config_byte.pixelGrid = ( (config_byte.pixelGrid < 3) ? ((config_byte.pixelGrid << 1) + 1) : 0); break;
                        case 4: config_byte.multGrid = !config_byte.multGrid; break;
                        case 5: config_byte.smoothing = ( (config_byte.smoothing < 2) ? (config_byte.smoothing + 1) : 0); break;
                        case 6: config_byte.color = !config_byte.color; break;
                        case 7: config_byte.freq = !config_byte.freq; break;
                        case 8: config_byte.padDisp = !config_byte.padDisp; break;
                    }
                    osd_write_config();
                } break;
                case CTRL_IN_B: {
                    config_byte = config_byte_bak;
                    osd_write_config();
                    retVal = false;
                    osd_idx = OSD_MIN_IDX;
                } break;
                case CTRL_IN_START: {
                    eeprom_update_byte((uint8_t *)(E2END-1), *(uint8_t *)(&config_byte));
                    retVal = false;
                    osd_idx = OSD_MIN_IDX;
                }break;
            }
            if(update_osd) {
                _delay_ms(10);
                fpga_transfer( (0x2000) | (osd_idx << 1) | (retVal ? 1 : 0) );
                _delay_ms(10);
                fpga_transfer( (0x2000) | (osd_idx << 1) | (retVal ? 1 : 0) );
            }
        }
    }
    return retVal;
}

bool pad_display_enabled(){
    return config_byte.padDisp;
}

void pad_display_update(uint16_t controller_data){
    uint16_t pad_display_word = 0x8000;
    if (controller_data & CTRL_IN_A)      pad_display_word |= PAD_DISP_A;
    if (controller_data & CTRL_IN_B)      pad_display_word |= PAD_DISP_B;
    if (controller_data & CTRL_IN_X)      pad_display_word |= PAD_DISP_B;
    if (controller_data & CTRL_IN_Y)      pad_display_word |= PAD_DISP_A;
    if (controller_data & CTRL_IN_UP)     pad_display_word |= PAD_DISP_UP;
    if (controller_data & CTRL_IN_DOWN)   pad_display_word |= PAD_DISP_DOWN;
    if (controller_data & CTRL_IN_LEFT)   pad_display_word |= PAD_DISP_LEFT;
    if (controller_data & CTRL_IN_RIGHT)  pad_display_word |= PAD_DISP_RIGHT;
    if (controller_data & CTRL_IN_L)      pad_display_word |= PAD_DISP_L;
    if (controller_data & CTRL_IN_R)      pad_display_word |= PAD_DISP_R;
    if (controller_data & CTRL_IN_START)  pad_display_word |= PAD_DISP_START;
    if (controller_data & CTRL_IN_SELECT) pad_display_word |= PAD_DISP_SELECT;
    fpga_transfer(pad_display_word);
}

//this function is empirically compensated to generate 10us pulses inside a for loop @ 8MHz
void fpga_write_bit(bool high) {
    if(shield_variant == MANCLOUD) {
        if(high) {
            MC_FPGA_COMM_PORT &= ~_BV(MC_FPGA_COMM_PIN);
            _delay_us(2);
            MC_FPGA_COMM_PORT |= _BV(MC_FPGA_COMM_PIN);
            _delay_us(5);
        } else {
            MC_FPGA_COMM_PORT &= ~_BV(MC_FPGA_COMM_PIN);
            _delay_us(7);
            MC_FPGA_COMM_PORT |= _BV(MC_FPGA_COMM_PIN);
        }
    } else if (shield_variant == CONSOLES4YOU) {
        if(high) {
            C4Y_FPGA_COMM_PORT &= ~_BV(C4Y_FPGA_COMM_PIN);
            _delay_us(4);
            C4Y_FPGA_COMM_PORT |= _BV(C4Y_FPGA_COMM_PIN);
            _delay_us(10);
        } else {
            C4Y_FPGA_COMM_PORT &= ~_BV(C4Y_FPGA_COMM_PIN);
            _delay_us(14);
            C4Y_FPGA_COMM_PORT |= _BV(C4Y_FPGA_COMM_PIN);
        }
    }
}

//transfer command byte to FPGA
void fpga_transfer(uint16_t data) {
    for(uint16_t i = 1; i != 0; i<<=1) {
        fpga_write_bit(data & i);
    }
}

/*
 * controller.h
 *
 *  Created on: 02.01.2022
 *      Author: ManCloud
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

/* === INCLUDES ============================================================= */
#include <avr/io.h>

#include <global.h>

/* === TYPES ================================================================ */

/* === MACROS =============================================================== */
#define CTRL_IN_A           (0x008)
#define CTRL_IN_B           (0x800)
#define CTRL_IN_Y           (0x400)
#define CTRL_IN_X           (0x004)
#define CTRL_IN_UP          (0x080)
#define CTRL_IN_DOWN        (0x040)
#define CTRL_IN_LEFT        (0x020)
#define CTRL_IN_RIGHT       (0x010)
#define CTRL_IN_L           (0x002)
#define CTRL_IN_R           (0x001)
#define CTRL_IN_START       (0x100)
#define CTRL_IN_SELECT      (0x200)
#define CTRL_DATA_MASK      (0xFFF)
#define CTRL_BT_CONNECTED   (0x8000)

#define GBA_OUT_A      (0x010)
#define GBA_OUT_B      (0x020)
#define GBA_OUT_UP     (0x001)
#define GBA_OUT_DOWN   (0x002)
#define GBA_OUT_LEFT   (0x004)
#define GBA_OUT_RIGHT  (0x008)
#define GBA_OUT_L      (0x040)
#define GBA_OUT_R      (0x080)
#define GBA_OUT_START  (0x100)
#define GBA_OUT_SELECT (0x200)
#define GBA_DATA_MASK  (0x3FF)

#define COMBO_IGR      (CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT)
#define COMBO_AVR_RST  (CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT | CTRL_IN_A | CTRL_IN_Y | CTRL_IN_LEFT | CTRL_IN_RIGHT)
#define ENABLE_OSD     (CTRL_IN_UP | CTRL_IN_R | CTRL_IN_L | CTRL_IN_SELECT)

/* === GLOBALS ============================================================== */

/* === PROTOTYPES =========================================================== */
void controller_init(void);
void controller_update(void);
void controller_map_data(void);
void set_controller_data(uint16_t data);
uint16_t get_controller_data(void);

#endif /* CONTROLLER_H_ */

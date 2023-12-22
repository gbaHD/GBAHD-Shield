/*
 * twiSlave.h
 *
 *  Created on: 13.01.2023
 *      Author: ManCloud
 */

#ifndef OSD_H_
#define OSD_D_

#include "global.h"
#include "controller.h"

typedef struct config_tag {
 uint8_t smoothing :2;
 uint8_t pixelGrid :2;
 uint8_t multGrid :1;
 uint8_t color :1;
 uint8_t freq :1;
 uint8_t padDisp :1;
} config_t;

#define PAD_DISP_A      (0x010)
#define PAD_DISP_B      (0x020)
#define PAD_DISP_UP     (0x001)
#define PAD_DISP_DOWN   (0x002)
#define PAD_DISP_LEFT   (0x004)
#define PAD_DISP_RIGHT  (0x008)
#define PAD_DISP_L      (0x040)
#define PAD_DISP_R      (0x080)
#define PAD_DISP_START  (0x100)
#define PAD_DISP_SELECT (0x200)
#define PAD_DISP_MASK  (0x3FF)

void osd_init(void);
void osd_enter(void);
bool osd_update(uint16_t buttons);
void pad_display_update(uint16_t data);
bool pad_display_enabled();



#endif //OSD_H_

#ifndef FW_H_
#define FW_H_

#include <SPI.h>
#define GBA_OUTPUT_EN  9
#define SNES_LATCH  10
#define DEBUG 0

#define GBA_POWER_SEL 2
#define COMM_PIN 3

#define TWI_VALID_TIMEOUT 10

#define CTRL_IN_A      (0x008)
#define CTRL_IN_B      (0x800)
#define CTRL_IN_Y      (0x400)
#define CTRL_IN_X      (0x004)
#define CTRL_IN_UP     (0x080)
#define CTRL_IN_DOWN   (0x040)
#define CTRL_IN_LEFT   (0x020)
#define CTRL_IN_RIGHT  (0x010)
#define CTRL_IN_L      (0x002)
#define CTRL_IN_R      (0x001)
#define CTRL_IN_START  (0x100)
#define CTRL_IN_SELECT (0x200)

#define COMBO_IGR      (CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT)
#define COMBO_AVR_RST  (CTRL_IN_R | CTRL_IN_L | CTRL_IN_START | CTRL_IN_SELECT | CTRL_IN_A | CTRL_IN_X | CTRL_IN_LEFT | CTRL_IN_RIGHT)
#define ENABLE_OSD     (CTRL_IN_UP | CTRL_IN_R | CTRL_IN_L | CTRL_IN_SELECT)

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

#endif //FW_H_

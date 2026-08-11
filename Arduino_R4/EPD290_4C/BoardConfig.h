#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

/**
 * Board wiring. This is the only file that changes when moving the sketch
 * to a different Arduino board -- the rest of the driver only knows these names.
**/

/**
 * GPIO config
**/
#define EPD_SCK_PIN     13
#define EPD_MOSI_PIN    11
#define EPD_CS_PIN      7
#define EPD_DC_PIN      6
#define EPD_RST_PIN     5
#define EPD_BUSY_PIN    4

#endif

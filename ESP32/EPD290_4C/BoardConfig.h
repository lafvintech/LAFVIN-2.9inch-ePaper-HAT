#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

/**
 * Board wiring. This is the only file that changes when moving the sketch
 * to a different ESP32 board -- the rest of the driver only knows these names.
**/

/**
 * GPIO config(ESP32S3)
**/
#define EPD_SCK_PIN     42
#define EPD_MOSI_PIN    41
#define EPD_CS_PIN      4
#define EPD_DC_PIN      5
#define EPD_RST_PIN     35
#define EPD_BUSY_PIN    36

// /**
//  * GPIO config(ESP32)
// **/
// #define EPD_SCK_PIN     4
// #define EPD_MOSI_PIN    2
// #define EPD_CS_PIN      5
// #define EPD_DC_PIN      18
// #define EPD_RST_PIN     19
// #define EPD_BUSY_PIN    21

#endif

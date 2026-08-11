#ifndef __EPD_PANEL_H_
#define __EPD_PANEL_H_

#include "DEV_Config.h"

/**
 * Panel model. Moving this sketch to a different 4-color e-Paper module
 * means updating WIDTH/HEIGHT here and the init register sequences in
 * EPD_Panel.cpp -- nothing else in the project needs to change.
**/

// Display resolution
#define EPD_WIDTH        128
#define EPD_HEIGHT       296

// Color
#define  EPD_BLACK   0x0
#define  EPD_WHITE   0x1
#define  EPD_YELLOW  0x2
#define  EPD_RED     0x3

void EPD_Init(void);
void EPD_Init_Fast(void);
void EPD_Clear(const UBYTE color);
void EPD_Display(const UBYTE *Image);
void EPD_Display_Image2LCD(const UBYTE *Image);
void EPD_Sleep(void);

#endif

/*****************************************************************************
* Product: SST example, 80x86, Turbo C++ 1.01
*
* Copyright (C) 2006 Miro Samek and Robert Ward. All rights reserved.
*
* MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

/* Direct Video Access .....................................................*/
enum VideoColor {
    /* foreground */
    VIDEO_FGND_BLACK        = 0x00,
    VIDEO_FGND_BLUE         = 0x01,
    VIDEO_FGND_GREEN        = 0x02,
    VIDEO_FGND_CYAN         = 0x03,
    VIDEO_FGND_RED          = 0x04,
    VIDEO_FGND_PURPLE       = 0x05,
    VIDEO_FGND_BROWN        = 0x06,
    VIDEO_FGND_LIGHT_GRAY   = 0x07,
    VIDEO_FGND_DARK_GRAY    = 0x08,
    VIDEO_FGND_LIGHT_BLUE   = 0x09,
    VIDEO_FGND_LIGHT_GREEN  = 0x0A,
    VIDEO_FGND_LIGHT_CYAN   = 0x0B,
    VIDEO_FGND_LIGHT_RED    = 0x0C,
    VIDEO_FGND_LIGHT_PURPLE = 0x0D,
    VIDEO_FGND_YELLOW       = 0x0E,
    VIDEO_FGND_WHITE        = 0x0F,
    /* background */
    VIDEO_BGND_BLACK        = 0x00,
    VIDEO_BGND_BLUE         = 0x10,
    VIDEO_BGND_GREEN        = 0x20,
    VIDEO_BGND_CYAN         = 0x30,
    VIDEO_BGND_RED          = 0x40,
    VIDEO_BGND_PURPLE       = 0x50,
    VIDEO_BGND_BROWN        = 0x60,
    VIDEO_BGND_LIGHT_GRAY   = 0x70,

    VIDEO_BGND_BLINK        = 0x80
};
/*..........................................................................*/
void Video_clearScreen(uint8_t bgColor);
void Video_clearRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint8_t bgColor);
void Video_printChAt(uint8_t x, uint8_t y, uint8_t color, char ch);
void Video_printStrAt(uint8_t x, uint8_t y, uint8_t color,
                      char const *str);
void Video_printNumAt(uint8_t x, uint8_t y, uint8_t color, uint32_t num);

#endif                                                             /* bsp_h */


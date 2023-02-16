/*============================================================================
* FreeRTOS Example (adaptation of the SST blinky_button example)
*
* Copyright (C) 2006-2023 Quantum Leaps, <state-machine.com>.
*
* SPDX-License-Identifier: MIT
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
============================================================================*/
#ifndef BSP_H_
#define BSP_H_

#define BSP_TICKS_PER_SEC configTICK_RATE_HZ

void BSP_init(void);

void BSP_d1on(void);
void BSP_d1off(void);

void BSP_d2on(void);
void BSP_d2off(void);

void BSP_d3on(void);
void BSP_d3off(void);

void BSP_d4on(void);
void BSP_d4off(void);

void BSP_d5on(void);
void BSP_d5off(void);

void BSP_d6on(void);
void BSP_d6off(void);

/* immutable events for Blinky tasks */
Event const *BSP_getWorkEvtBlinky1(uint8_t num);
Event const *BSP_getWorkEvtBlinky3(uint8_t num);

#endif /* BSP_H_ */

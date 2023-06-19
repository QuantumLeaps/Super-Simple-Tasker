/*============================================================================
* Super-Simple Tasker (SST/C) Example
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
#ifndef BLINKY_BUTTON_H_
#define BLINKY_BUTTON_H_

#include "dbc_assert.h" /* Design By Contract (DBC) assertions */

enum Signals {
    TIMEOUT_SIG,
    BUTTON_PRESSED_SIG,
    BUTTON_RELEASED_SIG,
    BLINKY_WORK_SIG,
    FORWARD_PRESSED_SIG,
    FORWARD_RELEASED_SIG,
    /* ... */
    MAX_SIG  /* the last signal */
};

typedef struct {
    SST_Evt super;    /* inherit SST_Evt */
    uint16_t toggles; /* number of toggles of the signal */
    uint8_t ticks;    /* number of clock ticks between */
} BlinkyWorkEvt;

typedef struct {
    SST_Evt super;    /* inherit SST_Evt */
    uint16_t toggles; /* number of toggles of the signal */
} ButtonWorkEvt;

void Blinky1_instantiate(void);
extern SST_Task * AO_Blinky1;  /* opaque task pointer */

void Blinky3_instantiate(void);
extern SST_Task * AO_Blinky3;  /* opaque task pointer */

void Button2a_instantiate(void);
extern SST_Task * AO_Button2a; /* opaque task pointer */

void Button2b_instantiate(void);
extern SST_Task * AO_Button2b; /* opaque task pointer */

#endif /* BLINKY_BUTTON_H_ */

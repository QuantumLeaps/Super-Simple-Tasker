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
#include "sst.h"           /* SST framework */
#include "bsp.h"           /* Board Support Package interface */
#include "blinky_button.h" /* application shared interface */

DBC_MODULE_NAME("button2b")

/*..........................................................................*/
typedef struct {    /* Button2b task */
    SST_Task super; /* inherit SST_Task */
    /* add internal variables for this AO... */
} Button2b;

static void Button2b_init(Button2b * const me, SST_Evt const * const e);
static void Button2b_dispatch(Button2b * const me, SST_Evt const * const e);

/*..........................................................................*/
static Button2b Button2b_inst; /* the Button2b instance */
SST_Task * const AO_Button2b = &Button2b_inst.super; /* opaque AO pointer */

/*..........................................................................*/
void Button2b_ctor(void) {
    Button2b * const me = &Button2b_inst;
    SST_Task_ctor(
       &me->super,
       (SST_Handler)&Button2b_init,
       (SST_Handler)&Button2b_dispatch);
}

/*..........................................................................*/
static void Button2b_init(Button2b * const me, SST_Evt const * const ie) {
    (void)me;
    (void)ie;
}
/*..........................................................................*/
static void Button2b_dispatch(Button2b * const me, SST_Evt const * const e) {
    switch (e->sig) {
        case FORWARD_PRESSED_SIG: {
            BSP_d3on();
            /* Button2b --> Blinky3 */
            SST_Task_post(AO_Blinky3, BSP_getWorkEvtBlinky3(1U));
            BSP_d3off();

            for (uint16_t i = SST_EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
                 i > 0U; --i)
            {
                BSP_d3on();
                BSP_d3off();
            }
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP_d3on();
            /* Button2b --> Blinky3 */
            SST_Task_post(AO_Blinky3, BSP_getWorkEvtBlinky3(0U));
            BSP_d3off();

            for (uint16_t i = SST_EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
                 i > 0U; --i)
            {
                BSP_d3on();
                BSP_d3off();
            }
            break;
        }
        default: {
            DBC_ERROR(500); /* unexpected event */
            break;
        }
    }
}

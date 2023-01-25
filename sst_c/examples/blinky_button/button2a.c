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

DBC_MODULE_NAME("button2a") /* for DBC assertions in this module */

/*..........................................................................*/
typedef struct {    /* Button2a active object */
    SST_Task super; /* inherit SST_Task */
    /* add internal variables for this AO... */
} Button2a;

static void Button2a_init(Button2a * const me, SST_Evt const * const ie);
static void Button2a_dispatch(Button2a * const me, SST_Evt const * const e);

/*..........................................................................*/
static Button2a Button2a_inst; /* the Button2a instance */
SST_Task * const AO_Button2a = &Button2a_inst.super; /* opaque AO pointer */

/*..........................................................................*/
void Button2a_ctor(void) {
    Button2a * const me = &Button2a_inst;
    SST_Task_ctor(
       &me->super,
       (SST_Handler)&Button2a_init,
       (SST_Handler)&Button2a_dispatch);
}

/*..........................................................................*/
static void Button2a_init(Button2a * const me, SST_Evt const * const ie) {
    (void)me;
    (void)ie;
}
/*..........................................................................*/
static void Button2a_dispatch(Button2a * const me, SST_Evt const * const e) {
    (void)me;
    switch (e->sig) {
        case BUTTON_PRESSED_SIG: {
            BSP_d4on();
            /* Button2a --> Blinky1 */
            SST_Task_post(AO_Blinky1, BSP_getWorkEvtBlinky1(1U));
            BSP_d4off();

            for (uint16_t i = SST_EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
                 i > 0U; --i)
            {
                BSP_d4on();
                BSP_d4off();
            }
            break;
        }
        case FORWARD_PRESSED_SIG: {
            BSP_d4on();
            /* immutable event can be forwarded to another Task */
            SST_Task_post(AO_Button2b, e); /* Button2a --> Button2b */
            BSP_d4off();
            break;
        }
        case BUTTON_RELEASED_SIG: {
            BSP_d4on();
            /* Button2a --> Blinky1 */
            SST_Task_post(AO_Blinky1, BSP_getWorkEvtBlinky1(0U));
            BSP_d4off();

            for (uint16_t i = SST_EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
                 i > 0U; --i)
            {
                BSP_d4on();
                BSP_d4off();
            }
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP_d4on();
            /* immutable event can be forwarded to another Task */
            SST_Task_post(AO_Button2b, e); /* Button2a --> Button2b */
            BSP_d4off();
            break;
        }
        default: {
            DBC_ERROR(500); /* unexpected event */
            break;
        }
    }
}

/*============================================================================
* Super-Simple Tasker (SST0/C) Example
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

DBC_MODULE_NAME("blinky1") /* for DBC assertions in this module */

/*..........................................................................*/
typedef struct {      /* Blinky1 task */
    SST_Task super;   /* inherit SST_Task */
    SST_TimeEvt te;   /* time event for generating TIMEOUT events */
    uint16_t toggles; /* number of toggles to perform for TIMEOUT event */
} Blinky1;

static void Blinky1_ctor(Blinky1 * const me);
static void Blinky1_init(Blinky1 * const me, SST_Evt const * const ie);
static void Blinky1_dispatch(Blinky1 * const me, SST_Evt const * const e);

/*..........................................................................*/
static Blinky1 Blinky1_inst; /* the Blinky instance */
SST_Task * const AO_Blinky1 = &Blinky1_inst.super; /* opaque AO pointer */

void Blinky1_instantiate(void) {
    Blinky1_ctor(&Blinky1_inst);
}

/*..........................................................................*/
static void Blinky1_ctor(Blinky1 * const me) {
    SST_Task_ctor(
       &me->super,
       (SST_Handler)&Blinky1_init,
       (SST_Handler)&Blinky1_dispatch);
    SST_TimeEvt_ctor(&me->te, TIMEOUT_SIG, &me->super);
}
/*..........................................................................*/
static void Blinky1_init(Blinky1 * const me, SST_Evt const * const ie) {
    /* the initial event must be provided and must be WORKLOAD_SIG */
    DBC_REQUIRE(300,
        (ie != (SST_Evt const *)0) && (ie->sig == BLINKY_WORK_SIG));

    SST_TimeEvt_arm(&me->te,
        SST_EVT_DOWNCAST(BlinkyWorkEvt, ie)->ticks,
        SST_EVT_DOWNCAST(BlinkyWorkEvt, ie)->ticks);
    me->toggles = SST_EVT_DOWNCAST(BlinkyWorkEvt, ie)->toggles;
}
/*..........................................................................*/
static void Blinky1_dispatch(Blinky1 * const me, SST_Evt const * const e) {
    switch (e->sig) {
        case TIMEOUT_SIG: {
            for (uint16_t i = me->toggles; i > 0U; --i) {
                /* SST scheduler lock is not needed in non-preemptive SST0 */
                //SST_LockKey key = SST_Task_lock(3U);
                BSP_d5on();
                BSP_d5off();
                //SST_Task_unlock(key);
            }
            break;
        }
        case BLINKY_WORK_SIG: {
            BSP_d5on();
            SST_TimeEvt_arm(&me->te,
                SST_EVT_DOWNCAST(BlinkyWorkEvt, e)->ticks,
                SST_EVT_DOWNCAST(BlinkyWorkEvt, e)->ticks);
            me->toggles = SST_EVT_DOWNCAST(BlinkyWorkEvt, e)->toggles;
            BSP_d5off();
            break;
        }
        default: {
            DBC_ERROR(500); /* unexpected event */
            break;
        }
    }
}

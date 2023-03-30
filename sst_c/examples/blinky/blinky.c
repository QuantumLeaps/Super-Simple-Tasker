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
#include "blinky.h"        /* application shared interface */

DBC_MODULE_NAME("blinky")  /* for DBC assertions in this module */

/* Blinky event-driven task ------------------------------------------------*/
typedef struct {
    SST_Task super; /* inherit SST_Task */

    SST_TimeEvt te1;
    SST_TimeEvt te2;
} Blinky;

static void Blinky_ctor(Blinky * const me);
static void Blinky_init(Blinky * const me, SST_Evt const * const ie);
static void Blinky_dispatch(Blinky * const me, SST_Evt const * const e);

/*..........................................................................*/
static Blinky Blinky_inst; /* the Blinky instance */
SST_Task * const AO_Blinky = &Blinky_inst.super; /* opaque AO pointer */

/*..........................................................................*/
void Blinky_instantiate(void) {
    Blinky_ctor(&Blinky_inst);
}
/*..........................................................................*/
void Blinky_ctor(Blinky * const me) {
    SST_Task_ctor(
       &me->super,
       (SST_Handler)&Blinky_init,
       (SST_Handler)&Blinky_dispatch);
    SST_TimeEvt_ctor(&me->te1, TIMEOUT1_SIG, &me->super);
    SST_TimeEvt_ctor(&me->te2, TIMEOUT2_SIG, &me->super);
}

/* macro to select the Blinky implementation */
#define BLINKY_IMPL 2

/*--------------------------------------------------------------------------*/
#if BLINKY_IMPL == 1
/* Blinky implementation closest matching the traditional blocking approach */

static void Blinky_init(Blinky * const me, SST_Evt const * const ie) {
    (void)ie; /* unused parameter */
    SST_TimeEvt_arm(&me->te1, 1U, 0U);
}
/*..........................................................................*/
static void Blinky_dispatch(Blinky * const me, SST_Evt const * const e) {
    switch (e->sig) {
        case TIMEOUT1_SIG:
            BSP_ledOn();
            SST_TimeEvt_arm(&me->te2, BSP_TICKS_PER_SEC / 4U, 0U);
            break;
        case TIMEOUT2_SIG:
            BSP_ledOff();
            SST_TimeEvt_arm(&me->te1, BSP_TICKS_PER_SEC * 3U/4U, 0U);
            break;
    }
}

/*--------------------------------------------------------------------------*/
#elif BLINKY_IMPL == 2
/* Blinky implementation with two periodic time events with offset */

static void Blinky_init(Blinky * const me, SST_Evt const * const ie) {
    (void)ie; /* unused parameter */
    SST_TimeEvt_arm(&me->te1, 1U,                          BSP_TICKS_PER_SEC);
    SST_TimeEvt_arm(&me->te2, 1U + (BSP_TICKS_PER_SEC/4U), BSP_TICKS_PER_SEC);
}
/*..........................................................................*/
static void Blinky_dispatch(Blinky * const me, SST_Evt const * const e) {
    switch (e->sig) {
        case TIMEOUT1_SIG:
            BSP_ledOn();
            break;
        case TIMEOUT2_SIG:
            BSP_ledOff();
            break;
    }
}

/*--------------------------------------------------------------------------*/
#else
    #error "Wrong definition of the macro BLINKY_VERSION"
#endif

/*============================================================================
* Blinky3 task (adapted for FreeRTOS)
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
#include "FreeRTOS.h"   /* Free Active Object interface */
#include "task.h"       /* FreeRTOS task API */
#include "queue.h"      /* FreeRTOS queue API */

#include "blinky_button.h" /* application shared interface */
#include "bsp.h"           /* Board Support Package interface */

/*..........................................................................*/
typedef struct {    /* Blinky3 active object */
    uint16_t toggles;
    uint8_t ticks;
    uint8_t tick_ctr;
} Blinky3;

static void Blinky3_init(Blinky3 * const me, Event const * const ie);
static void Blinky3_dispatch(Blinky3 * const me, Event const * const e);
static Blinky3 Blinky3_inst; /* the Blinky3 instance (Singleton) */

/*..........................................................................*/
void * const AO_Blinky3 = &Blinky3_inst; /* opaque AO pointer */
QueueHandle_t QUEUE_Blinky3; /* FreeRTOS queue "handle" */

/*..........................................................................*/
void Blinky3_task(void *pvParameters) {
    Blinky3 * const me = (Blinky3 *)pvParameters;
    Blinky3_init(me, BSP_getWorkEvtBlinky3(0U));

    while (1) {
        /* wait for any event and receive it into object 'e' */
        Event *e;
        xQueueReceive(QUEUE_Blinky3, &e, portMAX_DELAY); /* BLOCKING! */
        configASSERT(e != (Event *)0);
        Blinky3_dispatch(me, e);
    }
}

/*..........................................................................*/
static void Blinky3_init(Blinky3 * const me, Event const * const ie) {
    /* the initial event must be provided and must be WORKLOAD_SIG */
    configPRECONDITION((ie != (Event const *)0)
                       && (ie->sig == BLINKY_WORK_SIG));

    me->toggles = EVT_DOWNCAST(BlinkyWorkEvt, ie)->toggles;
    me->ticks = EVT_DOWNCAST(BlinkyWorkEvt, ie)->ticks;
    me->tick_ctr = me->ticks;
}
/*..........................................................................*/
static void Blinky3_dispatch(Blinky3 * const me, Event const * const e) {
    switch (e->sig) {
        case TICK_SIG: {
            --me->tick_ctr;
            if (me->tick_ctr == 0U) {
                me->tick_ctr = me->ticks;
                for (uint16_t i = me->toggles; i > 0U; --i) {
                    BSP_d2on();
                    BSP_d2off();
                }
            }
            break;
        }
        case BLINKY_WORK_SIG: {
            BSP_d2on();
            me->toggles = EVT_DOWNCAST(BlinkyWorkEvt, e)->toggles;
            me->ticks = EVT_DOWNCAST(BlinkyWorkEvt, e)->ticks;
            BSP_d2off();
            break;
        }
        default: {
            configASSERT(0); /* unexpected event */
            break;
        }
    }
}

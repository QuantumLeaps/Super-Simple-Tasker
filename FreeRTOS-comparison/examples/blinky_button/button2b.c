/*============================================================================
* Button2b task (adapted for FreeRTOS)
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
#include "FreeRTOS.h"   /* FreeRTOS API */
#include "task.h"       /* FreeRTOS task API */
#include "queue.h"      /* FreeRTOS queue API */

#include "blinky_button.h" /* application shared interface */
#include "bsp.h"           /* Board Support Package interface */

/*..........................................................................*/
typedef struct {    /* Button2b task */
    /* add internal variables for this AO... */
} Button2b;

static void Button2b_init(Button2b * const me, Event const * const e);
static void Button2b_dispatch(Button2b * const me, Event const * const e);
static Button2b Button2b_inst; /* the Button2b instance */

/*..........................................................................*/
void * const AO_Button2b = &Button2b_inst; /* opaque AO pointer */
QueueHandle_t QUEUE_Button2b; /* FreeRTOS queue "handle" */

/*..........................................................................*/
void Button2b_task(void *pvParameters) {
    Button2b * const me = (Button2b *)pvParameters;
    Button2b_init(me, (Event const *)0);

    while (1) {
        /* wait for any event and receive it into object 'e' */
        Event *e;
        xQueueReceive(QUEUE_Button2b, &e, portMAX_DELAY); /* BLOCKING! */

        Button2b_dispatch(me, e);
    }
}

/*..........................................................................*/
static void Button2b_init(Button2b * const me, Event const * const ie) {
    (void)me;
    (void)ie;
}
/*..........................................................................*/
static void Button2b_dispatch(Button2b * const me, Event const * const e) {
    BaseType_t qStatus;
    Event const *pe;
    switch (e->sig) {
        case FORWARD_PRESSED_SIG: {
            BSP_d3on();
            /* Button2b --> Blinky3 */
            pe = BSP_getWorkEvtBlinky3(1U);
            qStatus = xQueueSend(QUEUE_Blinky3,
                                 (void const *)&pe, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d3off();

            for (uint16_t i = EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
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
            pe = BSP_getWorkEvtBlinky3(0U);
            qStatus = xQueueSend(QUEUE_Blinky3,
                                 (void const *)&pe, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d3off();

            for (uint16_t i = EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
                 i > 0U; --i)
            {
                BSP_d3on();
                BSP_d3off();
            }
            break;
        }
        default: {
            configASSERT(0); /* unexpected event */
            break;
        }
    }
}

/*============================================================================
* Button2a task (adapted for FreeRTOS)
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
typedef struct {    /* Button2a task */
    /* add internal variables for this AO... */
} Button2a;

static void Button2a_init(Button2a * const me, Event const * const ie);
static void Button2a_dispatch(Button2a * const me, Event const * const e);
static Button2a Button2a_inst; /* the Button2a instance (Singleton) */

/*..........................................................................*/
void * const AO_Button2a = &Button2a_inst; /* opaque AO pointer */
QueueHandle_t QUEUE_Button2a; /* FreeRTOS queue "handle" */

/*..........................................................................*/
void Button2a_task(void *pvParameters) {
    Button2a * const me = (Button2a *)pvParameters;
    Button2a_init(me, (Event const *)0);

    while (1) {
        /* wait for any event and receive it into object 'e' */
        Event *e;
        xQueueReceive(QUEUE_Button2a, &e, portMAX_DELAY); /* BLOCKING! */
        configASSERT(e != (Event *)0);
        Button2a_dispatch(me, e);
    }
}

/*..........................................................................*/
static void Button2a_init(Button2a * const me, Event const * const ie) {
    (void)me;
    (void)ie;
}
/*..........................................................................*/
static void Button2a_dispatch(Button2a * const me, Event const * const e) {
    (void)me;
    BaseType_t qStatus;
    Event const *pe;
    switch (e->sig) {
        case BUTTON_PRESSED_SIG: {
            BSP_d4on();
            /* Button2a --> Blinky1 */
            pe = BSP_getWorkEvtBlinky1(1U);
            qStatus = xQueueSend(QUEUE_Blinky1,
                                 (void const *)&pe, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d4off();

            for (uint16_t i = EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
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
            /* Button2a --> Button2b */
            qStatus = xQueueSend(QUEUE_Button2b,
                                 (void const *)&e, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d4off();
            break;
        }
        case BUTTON_RELEASED_SIG: {
            BSP_d4on();
            /* Button2a --> Blinky1 */
            pe = BSP_getWorkEvtBlinky1(0U);
            qStatus = xQueueSend(QUEUE_Blinky1,
                                 (void const *)&pe, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d4off();

            for (uint16_t i = EVT_DOWNCAST(ButtonWorkEvt, e)->toggles;
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
            /* Button2a --> Button2b */
            qStatus = xQueueSend(QUEUE_Button2b,
                                 (void const *)&e, (TickType_t)0);
            configASSERT(qStatus == pdTRUE);
            BSP_d4off();
            break;
        }
        default: {
            configASSERT(0); /* unexpected event */
            break;
        }
    }
}

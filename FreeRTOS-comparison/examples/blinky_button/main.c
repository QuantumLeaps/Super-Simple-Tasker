/*============================================================================
* FreeRTOS Example
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
int main() {
    BSP_init(); /* initialize the Board Support Package */

    /* instantiate and start all FreeRTOS tasks and queues... */
    TaskHandle_t th;

    /* Blinky1... */
    static StaticQueue_t Blinky1_qcb; /* queue control block */
    static Event *Blinky1_QSto[10];   /* queue storage */
    QUEUE_Blinky1 = xQueueCreateStatic(
        ARRAY_NELEM(Blinky1_QSto),    /* queue length */
        sizeof(Event *),              /* item size */
        (uint8_t *)Blinky1_QSto,      /* queue storage */
        &Blinky1_qcb);                /* queue control block */
    configASSERT(QUEUE_Blinky1);

    static StaticTask_t Blinky1_tcb;  /* task control block */
    static StackType_t  Blinky1_stack[configMINIMAL_STACK_SIZE];
    th = xTaskCreateStatic(
        &Blinky1_task,                /* task function */
        "Blinky1",                    /* task priority */
        ARRAY_NELEM(Blinky1_stack),   /* stack length */
        AO_Blinky1,                   /* task param */
        1U + tskIDLE_PRIORITY,        /* task priority */
        Blinky1_stack,                /* task stack */
        &Blinky1_tcb);                /* task control block */
    configASSERT(th);

    /* Button2a... */
    static StaticQueue_t Button2a_qcb;/* queue control block */
    static Event *Button2a_QSto[10];  /* queue storage */
    QUEUE_Button2a = xQueueCreateStatic(
        ARRAY_NELEM(Button2a_QSto),   /* queue length */
        sizeof(Event *),              /* item size */
        (uint8_t *)Button2a_QSto,     /* queue storage */
        &Button2a_qcb);               /* queue control block */
    configASSERT(QUEUE_Button2a);

    static StaticTask_t Button2a_tcb; /* task control block */
    static StackType_t  Button2a_stack[configMINIMAL_STACK_SIZE];
    th = xTaskCreateStatic(
        &Button2a_task,               /* task function */
        "Button2a",                   /* task priority */
        ARRAY_NELEM(Button2a_stack),  /* stack length */
        AO_Button2a,                  /* task param */
        2U + tskIDLE_PRIORITY,        /* task priority */
        Button2a_stack,               /* task stack */
        &Button2a_tcb);               /* task control block */
    configASSERT(th);

    /* Button2b... */
    static StaticQueue_t Button2b_qcb;/* queue control block */
    static Event *Button2b_QSto[10];  /* queue storage */
    QUEUE_Button2b = xQueueCreateStatic(
        ARRAY_NELEM(Button2b_QSto),   /* queue length */
        sizeof(Event *),              /* item size */
        (uint8_t *)Button2b_QSto,     /* queue storage */
        &Button2b_qcb);               /* queue control block */
    configASSERT(QUEUE_Button2b);

    static StaticTask_t Button2b_tcb; /* task control block */
    static StackType_t  Button2b_stack[configMINIMAL_STACK_SIZE];
    th = xTaskCreateStatic(
        &Button2b_task,               /* task function */
        "Button2b",                   /* task priority */
        ARRAY_NELEM(Button2b_stack),  /* stack length */
        AO_Button2b,                  /* task param */
        2U + tskIDLE_PRIORITY,        /* task priority */
        Button2b_stack,               /* task stack */
        &Button2b_tcb);               /* task control block */
    configASSERT(th);

    /* Blinky3... */
    static StaticQueue_t Blinky3_qcb; /* queue control block */
    static Event *Blinky3_QSto[10];   /* queue storage */
    QUEUE_Blinky3 = xQueueCreateStatic(
        ARRAY_NELEM(Blinky3_QSto),    /* queue length */
        sizeof(Event *),              /* item size */
        (uint8_t *)Blinky3_QSto,      /* queue storage */
        &Blinky3_qcb);                /* queue control block */
    configASSERT(QUEUE_Blinky3);

    static StaticTask_t Blinky3_tcb;  /* task control block */
    static StackType_t  Blinky3_stack[configMINIMAL_STACK_SIZE];
    th = xTaskCreateStatic(
        &Blinky3_task,                /* task function */
        "Blinky3",                    /* task priority */
        ARRAY_NELEM(Blinky3_stack),   /* stack length */
        AO_Blinky3,                   /* task param */
        3U + tskIDLE_PRIORITY,        /* task priority */
        Blinky3_stack,                /* task stack */
        &Blinky3_tcb);                /* task control block */
    configASSERT(th);

    vTaskStartScheduler(); /* start the FreeRTOS scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}


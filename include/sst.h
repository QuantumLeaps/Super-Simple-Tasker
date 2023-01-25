/*===========================================================================
* Super-Simple Tasker (SST/C) API
*
* Copyright (C) 2005-2023 Quantum Leaps, <state-machine.com>.
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
===========================================================================*/
#ifndef SST_H_
#define SST_H_

#include <stdint.h>   /* standard C99 integer types */
#include "sst_port.h" /* SST port for specific CPU */

/* SST Event facilities ----------------------------------------------------*/
/*! signal of SST event */
typedef uint16_t SST_Signal;

/*! SST event class */
typedef struct {
    SST_Signal sig;
} SST_Evt;

/*! macro for downcasting SST events to specific Evt "subclasses" */
#define SST_EVT_DOWNCAST(EVT_, e_) ((EVT_ const *)(e_))

/* SST Task facilities -----------------------------------------------------*/
typedef struct SST_Task SST_Task; /* forward declaration */

/*! SST Task priority */
typedef uint8_t SST_TaskPrio;

/*! SST internal event-queue counter */
typedef uint8_t SST_QCtr;

/*! generic handler signature */
typedef void (*SST_Handler)(SST_Task * const me, SST_Evt const * const e);

/*! SST task (a.k.a. "Active Object") */
struct SST_Task {
    SST_Handler init;
    SST_Handler dispatch;

    SST_Evt const **qBuf; /*!< ring buffer for the queue */
    SST_QCtr end;   /*!< last index into the ring buffer */
    SST_QCtr head;  /*!< index for inserting events */
    SST_QCtr tail;  /*!< index for removing events */
    SST_QCtr nUsed; /*!< # used entries currently in the queue */

#ifdef SST_PORT_TASK_ATTR
    SST_PORT_TASK_ATTR
#endif
};

void SST_Task_ctor(
    SST_Task * const me,
    SST_Handler init,
    SST_Handler dispatch);

void SST_Task_start(
    SST_Task * const me,
    SST_TaskPrio prio,
    SST_Evt const **qBuf, SST_QCtr qLen,
    SST_Evt const * const ie);

void SST_Task_post(SST_Task * const me, SST_Evt const * const e);

int  SST_Task_run(void); /* run SST tasks static */

#ifdef SST_PORT_TASK_OPER
    /* additional Task operations needed by the specific SST port */
    SST_PORT_TASK_OPER
#endif

/* SST Kernel facilities ---------------------------------------------------*/
void SST_init(void);
void SST_onStart(void);

#ifndef SST_LOG2
static inline uint_fast8_t SST_LOG2(uint32_t x) {
    static uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    uint_fast8_t n = 0U;
    SST_ReadySet tmp;

    #if (SST_PORT_MAX_TASK > 16U)
    tmp = (SST_ReadySet)(x >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x = tmp;
    }
    #endif
    #if (SST_PORT_MAX_TASK > 8U)
    tmp = (x >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x = tmp;
    }
    #endif
    tmp = (x >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x = tmp;
    }
    return n + log2LUT[x];
}
#endif

/* general convenience utilities -------------------------------------------*/
#ifndef ARRAY_NELEM
/*! convenience macro to provide the number of elements in the array a_ */
#define ARRAY_NELEM(a_)  (sizeof(a_) / sizeof((a_)[0]))
#endif /* ARRAY_NELEM */

#endif /* SST_H_ */

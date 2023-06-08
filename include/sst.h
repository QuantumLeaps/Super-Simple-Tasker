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

#include <stdint.h>   /* standard C99 integers */
#include <stdbool.h>  /* standard C99 Boolean */
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

/* lock the SST task scheduler up to the provided priority ceiling (SRP) */
SST_LockKey SST_Task_lock(SST_TaskPrio ceiling);

/* unlock the SST task scheduler with the provided lock key */
void SST_Task_unlock(SST_LockKey lock_key);

/* SST Time Event facilities -----------------------------------------------*/
/*! SST internal time-event tick counter */
typedef uint16_t SST_TCtr;

/*! SST time event class */
typedef struct SST_TimeEvt SST_TimeEvt;
struct SST_TimeEvt {
    SST_Evt super;

    SST_TimeEvt *next; /*! link to next time event in a link-list */
    SST_Task *task;    /*! the owner task to post time event to */
    SST_TCtr ctr;      /*! time event down-counter */
    SST_TCtr interval; /*! interval for periodic time event */
};

void SST_TimeEvt_ctor(
    SST_TimeEvt * const me,
    SST_Signal sig,
    SST_Task *task);

void SST_TimeEvt_arm(
    SST_TimeEvt * const me,
    SST_TCtr ctr,
    SST_TCtr interval);

bool SST_TimeEvt_disarm(
    SST_TimeEvt * const me);

void SST_TimeEvt_tick(void); /* static handle all instantiated time events */

/* SST Kernel facilities ---------------------------------------------------*/
void SST_init(void);
void SST_start(void);
void SST_onStart(void);

/* general convenience utilities -------------------------------------------*/
#ifndef ARRAY_NELEM
/*! convenience macro to provide the number of elements in the array a_ */
#define ARRAY_NELEM(a_)  (sizeof(a_) / sizeof((a_)[0]))
#endif /* ARRAY_NELEM */

#endif /* SST_H_ */

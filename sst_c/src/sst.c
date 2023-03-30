/*===========================================================================
* Super-Simple Tasker (SST/C)
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
===========================================================================*/
#include "sst.h"        /* Super-Simple Tasker (SST) */
#include "dbc_assert.h" /* Design By Contract (DBC) assertions */

DBC_MODULE_NAME("sst")  /* for DBC assertions in this module */

/*..........................................................................*/
int SST_Task_run(void) {
    SST_start();   /* port-specific start of multitasking */
    SST_onStart(); /* application callback to config & start interrupts */

    for (;;) { /* idle loop of the SST kernel */
        SST_onIdle();
    }
}

/*--------------------------------------------------------------------------*/
void SST_Task_ctor(
    SST_Task * const me,
    SST_Handler init,
    SST_Handler dispatch)
{
    me->init = init;
    me->dispatch = dispatch;
}
/*..........................................................................*/
void SST_Task_start(
    SST_Task * const me,
    SST_TaskPrio prio,
    SST_Evt const **qBuf, SST_QCtr qLen,
    SST_Evt const * const ie)
{
    /*! @pre
    * - the priority must be greater than zero
    * - the queue storage and length must be provided
    */
    DBC_REQUIRE(200,
        (0U < prio)
        && (qBuf != (SST_Evt const **)0) && (qLen > 0U));

    me->qBuf  = qBuf;
    me->end   = qLen - 1U;
    me->head  = 0U;
    me->tail  = 0U;
    me->nUsed = 0U;

    SST_Task_setPrio(me, prio);

    /* initialize this task with the initialization event */
    (*me->init)(me, ie); /* NOTE: virtual call */
    /* TBD: implement event recycling */
}
/*..........................................................................*/
void SST_Task_post(SST_Task * const me, SST_Evt const * const e) {
    /*! @pre the queue must be sized adequately and cannot overflow */
    DBC_REQUIRE(300, me->nUsed <= me->end);

    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    me->qBuf[me->head] = e; /* insert event into the queue */
    if (me->head == 0U) {   /* need to wrap the head? */
        me->head = me->end; /* wrap around */
    }
    else {
        --me->head;
    }
    ++me->nUsed;
    SST_PORT_TASK_PEND();
    SST_PORT_CRIT_EXIT();
}

/*--------------------------------------------------------------------------*/
static SST_TimeEvt *timeEvt_head = (SST_TimeEvt *)0;

/*..........................................................................*/
void SST_TimeEvt_ctor(
    SST_TimeEvt * const me,
    SST_Signal sig,
    SST_Task *task)
{
    me->super.sig = sig;
    me->task = task;
    me->ctr = 0U;
    me->interval = 0U;

    /* insert time event "me" into the linked-list */
    me->next = timeEvt_head;
    timeEvt_head = me;
}
/*..........................................................................*/
void SST_TimeEvt_arm(
    SST_TimeEvt * const me,
    SST_TCtr ctr,
    SST_TCtr interval)
{
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    me->ctr = ctr;
    me->interval = interval;
    SST_PORT_CRIT_EXIT();
}
/*..........................................................................*/
bool SST_TimeEvt_disarm(SST_TimeEvt * const me) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    bool status = (me->ctr != 0U);
    me->ctr = 0U;
    me->interval = 0U;
    SST_PORT_CRIT_EXIT();
    return status;
}
/*..........................................................................*/
void SST_TimeEvt_tick(void) {
    for (SST_TimeEvt *t = timeEvt_head;
         t != (SST_TimeEvt *)0;
         t = t->next)
    {
        SST_PORT_CRIT_STAT
        SST_PORT_CRIT_ENTRY();
        if (t->ctr == 0U) { /* disarmed? (most frequent case) */
            SST_PORT_CRIT_EXIT();
        }
        else if (t->ctr == 1U) { /* expiring? */
            t->ctr = t->interval;
            SST_PORT_CRIT_EXIT();

            SST_Task_post(t->task, &t->super);
        }
        else { /* timing out */
            --t->ctr;
            SST_PORT_CRIT_EXIT();
        }
    }
}

/*===========================================================================
* Super-Simple Tasker (SST0/C)
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
#include "sst.h"     /* Super-Simple Tasker (SST) API */
#include "qassert.h" /* assertions for embedded systems */

Q_DEFINE_THIS_MODULE("sst0")

/* bitmask of all SST tasks in the system */
static SST_ReadySet task_readySet;

/* array of all SST task pointers in the system */
static SST_Task *task_registry[SST_PORT_MAX_TASK + 1U];

/*..........................................................................*/
void SST_init(void) {
}
/*..........................................................................*/
int SST_Task_run(void) {
    SST_onStart(); /* configure and start the interrupts */

    SST_PORT_INT_DISABLE();
    for (;;) { /* event loop of the SST0 kernel */

        if (task_readySet != 0U) { /* any SST tasks ready to run? */
            uint_fast8_t const p = SST_LOG2(task_readySet);
            SST_Task * const task = task_registry[p];
            SST_PORT_INT_ENABLE();

            /* the task must have some events in the queue */
            Q_ASSERT_ID(100, task->nUsed > 0U);

            /* get the event out of the queue */
            /* NOTE: no critical section because task->tail is accessed only
            * from this task
            */
            SST_Evt const *e = task->qBuf[task->tail];
            if (task->tail == 0U) { /* need to wrap the tail? */
                task->tail = task->end; /* wrap around */
            }
            else {
                --task->tail;
            }
            SST_PORT_INT_DISABLE();
            if ((--task->nUsed) == 0U) { /* no more events in the queue? */
                task_readySet &= ~(1U << (p - 1U));
            }
            SST_PORT_INT_ENABLE();

            /* dispatch the received event to the task */
            (*task->dispatch)(task, e); /* NOTE: virtual call */
            /* TBD: implement event recycling */
        }
        else { /* no SST tasks are ready to run --> idle */

            /* SST_onIdleCond() must be called with interrupts DISABLED
            * because the determination of the idle condition (all event
            * queues empty) can change at any time by an interrupt posting
            * events to a queue.
            *
            * NOTE: SST_onIdleCond() MUST enable interrupts internally,
            * ideally at the same time as putting the CPU into a power-
            * saving mode.
            */
            SST_onIdleCond();

            SST_PORT_INT_DISABLE(); /* disable before looping back */
        }
    }
#ifdef __GNUC__ /* GNU compiler? */
    return 0;
#endif
}
/*..........................................................................*/
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
    * - the priority must be in range
    * - the queue storage must be provided
    * - the queue length must not be zero
    * - the priority must not be in use
    */
    Q_REQUIRE_ID(300,
        (0U < prio) && (prio <= SST_PORT_MAX_TASK)
        && (qBuf != (SST_Evt const **)0) && (qLen > 0U)
        && (task_registry[prio] == (SST_Task *)0));

    me->prio  = prio;
    me->qBuf  = qBuf;
    me->end   = qLen - 1U;
    me->head  = 0U;
    me->tail  = 0U;
    me->nUsed = 0U;

    task_registry[prio] = me;

    /* initialize this task with the initialization event */
    (*me->init)(me, ie); /* NOTE: virtual call */
    /* TBD: implement event recycling */
}
/*..........................................................................*/
void SST_Task_post(SST_Task * const me, SST_Evt const * const e) {
    /*! @pre the queue must be sized adequately and cannot overflow */
    Q_REQUIRE_ID(400, me->nUsed <= me->end);

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
    task_readySet |= (1U << (me->prio - 1U));
    SST_PORT_CRIT_EXIT();
}

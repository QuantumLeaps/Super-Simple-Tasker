/*****************************************************************************
* SST platform-independent implementation
*
* Copyright (C) 2006 Miro Samek and Robert Ward. All rights reserved.
*
* MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*****************************************************************************/
#include "sst_port.h"

/* Public-scope objects ----------------------------------------------------*/
uint8_t SST_currPrio_ = (uint8_t)0xFF;              /* current SST priority */
uint8_t SST_readySet_ = (uint8_t)0;                        /* SST ready-set */

typedef struct TaskCBTag TaskCB;
struct TaskCBTag {
    SSTTask task__;
    SSTEvent *queue__;
    uint8_t end__;
    uint8_t head__;
    uint8_t tail__;
    uint8_t nUsed__;
    uint8_t mask__;
};

/* Local-scope objects -----------------------------------------------------*/
static TaskCB l_taskCB[SST_MAX_PRIO];

/*..........................................................................*/
void SST_task(SSTTask task, uint8_t prio, SSTEvent *queue, uint8_t qlen,
              SSTSignal sig, SSTParam par)
{
    SSTEvent ie;                                    /* initialization event */
    TaskCB *tcb  = &l_taskCB[prio - 1];
    tcb->task__  = task;
    tcb->queue__ = queue;
    tcb->end__   = qlen;
    tcb->head__  = (uint8_t)0;
    tcb->tail__  = (uint8_t)0;
    tcb->nUsed__ = (uint8_t)0;
    tcb->mask__  = (1 << (prio - 1));
    ie.sig = sig;
    ie.par = par;
    tcb->task__(ie);                                 /* initialize the task */
}
/*..........................................................................*/
void SST_run(void) {
    SST_start();                                              /* start ISRs */

    SST_INT_LOCK();
    SST_currPrio_ = (uint8_t)0;   /* set the priority for the SST idle loop */
    SST_schedule_();                  /* process all events produced so far */
    SST_INT_UNLOCK();

    for (;;) {                                         /* the SST idle loop */
        SST_onIdle();                        /* invoke the on-idle callback */
    }
}
/*..........................................................................*/
uint8_t SST_post(uint8_t prio, SSTSignal sig, SSTParam par) {
    TaskCB *tcb = &l_taskCB[prio - 1];
    SST_INT_LOCK();
    if (tcb->nUsed__ < tcb->end__) {
        tcb->queue__[tcb->head__].sig = sig;/* insert the event at the head */
        tcb->queue__[tcb->head__].par = par;
        if ((++tcb->head__) == tcb->end__) {
            tcb->head__ = (uint8_t)0;                      /* wrap the head */
        }
        if ((++tcb->nUsed__) == (uint8_t)1) {           /* the first event? */
            SST_readySet_ |= tcb->mask__;   /* insert task to the ready set */
            SST_schedule_();            /* check for synchronous preemption */
        }
        SST_INT_UNLOCK();
        return (uint8_t)1;                     /* event successfully posted */
    }
    else {
        SST_INT_UNLOCK();
        return (uint8_t)0;              /* queue full, event posting failed */
    }
}
/*..........................................................................*/
uint8_t SST_mutexLock(uint8_t prioCeiling) {
    uint8_t p;
    SST_INT_LOCK();
    p = SST_currPrio_;               /* the original SST priority to return */
    if (prioCeiling > SST_currPrio_) {
        SST_currPrio_ = prioCeiling;              /* raise the SST priority */
    }
    SST_INT_UNLOCK();
    return p;
}
/*..........................................................................*/
void SST_mutexUnlock(uint8_t orgPrio) {
    SST_INT_LOCK();
    if (orgPrio < SST_currPrio_) {
        SST_currPrio_ = orgPrio;    /* restore the saved priority to unlock */
        SST_schedule_(); /* the scheduler unlocks the interrupts internally */
    }
    SST_INT_UNLOCK();
}
/*..........................................................................*/
/* NOTE: the SST scheduler is entered and exited with interrupts LOCKED */
void SST_schedule_(void) {
    static uint8_t const log2Lkup[] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
    };
    uint8_t pin = SST_currPrio_;               /* save the initial priority */
    uint8_t p;                                          /* the new priority */
                            /* is the new priority higher than the initial? */
    while ((p = log2Lkup[SST_readySet_]) > pin) {
        TaskCB *tcb  = &l_taskCB[p - 1];
                                          /* get the event out of the queue */
        SSTEvent e = tcb->queue__[tcb->tail__];
        if ((++tcb->tail__) == tcb->end__) {
            tcb->tail__ = (uint8_t)0;
        }
        if ((--tcb->nUsed__) == (uint8_t)0) {/* is the queue becoming empty?*/
            SST_readySet_ &= ~tcb->mask__;     /* remove from the ready set */
        }
        SST_currPrio_ = p;        /* this becomes the current task priority */
        SST_INT_UNLOCK();                          /* unlock the interrupts */

        (*tcb->task__)(e);                             /* call the SST task */

        SST_INT_LOCK();            /* lock the interrupts for the next pass */
    }
    SST_currPrio_ = pin;                    /* restore the initial priority */
}

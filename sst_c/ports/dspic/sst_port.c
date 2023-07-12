/**
 * @file sst_port.c
 * @author ASHRAF (ashraf-masoud@elarabygroup.com)
 * @version 0.1
 * @date 2023-06-20
 */

/*===========================================================================
* Super-Simple Tasker (SST/C) port to dsPIC33ep
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
#include "sst.h"     /* Super-Simple Tasker (SST/C) */
#include "dbc_assert.h" /* Design By Contract (DBC) assertions */
#include "sst_port.h"

DBC_MODULE_NAME("sst_port") /* for DBC assertions in this module */

#define IFS_BASE     ((uint16_t volatile *)0x800U)  //interrupt flag status (pend)
#define IEC_BASE     ((uint16_t volatile *)0x820U)  //interrupt enable control
#define IPC_BASE     ((uint16_t volatile *)0x840U)  //interrupt priority control

static void interrupt_SetPriority(uint16_t IRQn, uint8_t priority)
{
    //CLR
    IPC_BASE[IRQn/4] &= ~(0X7<< ((IRQn%4)*4));
    //SET
    IPC_BASE[IRQn/4] |= priority << ((IRQn%4)*4);
}

static void interrupt_Enable(uint16_t IRQn)
{
    IEC_BASE[IRQn/16] |= 1 << (IRQn%16);
}
/*..........................................................................*/
//critical section counter
uint8_t g_crit_counter = 0;
/* SST kernel facilities ---------------------------------------------------*/
void SST_init(void) {
}
/*..........................................................................*/
void SST_start(void) {
}

/* SST Task facilities -----------------------------------------------------*/
void SST_Task_setPrio(SST_Task * const me, SST_TaskPrio prio) {

    /*! @pre
    * - the IRQ number must be already set
    * - the priority must fit in the NVIC
    */
    DBC_REQUIRE(200,
                (me->interrupt_num >= 0U)
                && (prio <= 0b111 && prio > 0));

    /* convert the SST direct priority (1,2,..) to NVIC priority... */
    //no need for dspic -> 1 lowest -> 7 highest

    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    /* set the Task priority of the associated IRQ */
    interrupt_SetPriority(me->interrupt_num, prio);

    /* enable the IRQ associated with the Task */
    interrupt_Enable(me->interrupt_num);
    SST_PORT_CRIT_EXIT();

    /* store the address of NVIC_PEND address and the IRQ bit */
    me->interrupt_pend = &IFS_BASE[me->interrupt_num/16 ];
    me->interrupt_num  = 1<<(me->interrupt_num%16);
}
/*..........................................................................*/
void SST_Task_activate(SST_Task * const me) {
    /*! @pre the queue must have some events */
    DBC_REQUIRE(300, me->nUsed > 0U);

    /* get the event out of the queue */
    /* NOTE: no critical section because me->tail is accessed only
    * from this task
    */
    SST_Evt const *e = me->qBuf[me->tail];
    if (me->tail == 0U) { /* need to wrap the tail? */
        me->tail = me->end; /* wrap around */
    }
    else {
        --me->tail;
    }
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    if ((--me->nUsed) > 0U) { /* some events still present in the queue? */
        *me->interrupt_pend |= me->interrupt_num; /* <=== pend the associated IRQ */
    }
    SST_PORT_CRIT_EXIT();

    /* dispatch the received event to this task */
    (*me->dispatch)(me, e); /* NOTE: virtual call */
    /* TBD: implement event recycling */
}
/*..........................................................................*/
void SST_Task_setIRQ(SST_Task * const me, uint8_t irq) 
{
    me->interrupt_num = irq;
}


/*..........................................................................*/
SST_LockKey SST_Task_lock(SST_TaskPrio ceiling) {
    /* NOTE:
    * dsPIC support the Interrupt Priority Level -IPL- bits in Status Reg -SR-
    * and the selective SST scheduler locking 
    * is implemented by setting IPL bits to the ceiling level.
    */
    // save current Priority
    SST_LockKey basepri_ = SRbits.IPL;
    // Current base priority is lower than the ceiling
    if(basepri_ < ceiling)
    {
        // update the Processor Priority
        SRbits.IPL = ceiling;
    }
    else
    {
        //do nothing
    }
    
    return basepri_;
}

/*..........................................................................*/
void SST_Task_unlock(SST_LockKey lock_key) 
{
    SRbits.IPL = lock_key;
}

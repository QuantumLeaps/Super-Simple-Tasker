/*===========================================================================
* Super-Simple Tasker (SST) port for ARM Cortex-M
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
#include "sst.h"     /* Super-Simple Tasker (SST) */
#include "qassert.h" /* assertions for embedded systems */

Q_DEFINE_THIS_MODULE("sst_port")

#define NVIC_PEND    ((uint32_t volatile *)0xE000E200U)
#define NVIC_EN      ((uint32_t volatile *)0xE000E100U)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400U)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14U)

/*..........................................................................*/
/* # of unused interrupt priority bits in NVIC */
static uint32_t nvic_prio_shift;

/* SST kernel facilities ---------------------------------------------------*/
void SST_init(void) {
    /* determine number of NVIC priority bits by writing 0xFF to the
    * NIVIC IP register for PendSV and then reading back the result,
    * which has only the implemented bits set.
    */
    uint32_t tmp = SCB_SYSPRI[3];
    SCB_SYSPRI[3] |= (0xFFU << 16U); /* write 0xFF to PendSV prio */
    uint32_t prio = ((SCB_SYSPRI[3] >> 16U) & 0xFFU); /* read back */
    SCB_SYSPRI[3] = tmp; /* restore the original PendSV prio */

    for (tmp = 0U; tmp < 8U; ++tmp) {
        if ((prio & (1U << tmp)) != 0U) {
            break;
        }
    }
    nvic_prio_shift = tmp;
}
/* SST Task facilities -----------------------------------------------------*/
void SST_Task_setPrio(SST_Task * const me, SST_TaskPrio prio) {

    /*! @pre the IRQ number must be already set */
    Q_REQUIRE_ID(200, me->nvic_irq != 0U);

    /* convert the SST direct priority (1,2,..) to NVIC priority... */
    uint32_t irq_prio = ((0xFFU >> nvic_prio_shift) + 1U - prio)
                         << nvic_prio_shift;

    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    /* set the Task priority of the associated IRQ */
    uint32_t tmp = NVIC_IP[me->nvic_irq >> 2U];
    tmp &= ~(0xFFU << ((me->nvic_irq & 3U) << 3U));
    tmp |= (irq_prio << ((me->nvic_irq & 3U) << 3U));
    NVIC_IP[me->nvic_irq >> 2U] = tmp;

    /* enable the IRQ associated with the Task */
    NVIC_EN[me->nvic_irq >> 5U] = (1U << (me->nvic_irq & 0x1FU));
    SST_PORT_CRIT_EXIT();

    /* store the address of NVIC_PEND address and the IRQ bit */
    me->nvic_pend = &NVIC_PEND[me->nvic_irq >> 5U];
    me->nvic_irq  = (1U << (me->nvic_irq & 0x1FU));
}
/*..........................................................................*/
void SST_Task_activate(SST_Task * const me) {
    /*! @pre the queue must have some events */
    Q_REQUIRE_ID(500, me->nUsed > 0U);

    /* get the event out of the queue */
    /* NOTE: no critical section because me->tail is accessed only
    * from this task
    */
    void const *e = me->qBuf[me->tail];
    if (me->tail == 0U) { /* need to wrap the tail? */
        me->tail = me->end; /* wrap around */
    }
    else {
        --me->tail;
    }
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    if ((--me->nUsed) > 0U) { /* some events still present in the queue? */
        *me->nvic_pend = me->nvic_irq; /* <=== pend the associated IRQ */
    }
    SST_PORT_CRIT_EXIT();

    /* dispatch the received event to this task */
    (*me->dispatch)(me, e); /* NOTE: virtual call */
    /* TBD: implement event recycling */
}
/*..........................................................................*/
void SST_Task_setIRQ(SST_Task * const me, uint8_t irq) {
    me->nvic_irq = irq;
}

/*===========================================================================
* Super-Simple Tasker (SST/C) port to ARM Cortex-M
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
#ifndef SST_PORT_H_
#define SST_PORT_H_

/* additional SST-PORT task attributes for ARM Cortex-M */
#define SST_PORT_TASK_ATTR \
    uint32_t volatile *nvic_pend; \
    uint32_t nvic_irq;

/* additional SST-PORT task operations for ARM Cortex-M */
#define SST_PORT_TASK_OPER \
    void SST_Task_activate(SST_Task * const me); \
    void SST_Task_setIRQ(SST_Task * const me, uint8_t irq); \
    void SST_Task_setPrio(SST_Task * const me, SST_TaskPrio prio);

/* SST-PORT critical section */
#define SST_PORT_CRIT_STAT
#define SST_PORT_CRIT_ENTRY() __asm volatile ("cpsid i")
#define SST_PORT_CRIT_EXIT()  __asm volatile ("cpsie i")

/* SST-PORT pend the Task after posting an event
* NOTE: executed inside SST critical section.
*/
#define SST_PORT_TASK_PEND()  (*me->nvic_pend = me->nvic_irq)

/* the idle SST callback for this SST port */
void SST_onIdle(void);

/* the SST scheduler lock key type */
typedef uint32_t SST_LockKey;

#endif /* SST_PORT_H_ */

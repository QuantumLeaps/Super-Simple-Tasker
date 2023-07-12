/**
 * @file sst_port.h
 * @author ASHRAF (ashrafkamel491@gmail.com)
 * @version 0.1
 * @date 2023-06-20
 */

/*===========================================================================
* Super-Simple Tasker (SST/C) port to DSPIC33ep
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
#include "xc.h"

/* additional SST-PORT task attributes for ARM Cortex-M */
#define SST_PORT_TASK_ATTR \
    uint16_t volatile *interrupt_pend; \
    uint16_t interrupt_num;

/* additional SST-PORT task operations for ARM Cortex-M */
#define SST_PORT_TASK_OPER \
    void SST_Task_activate(SST_Task * const me); \
    void SST_Task_setIRQ(SST_Task * const me, uint8_t irq); \
    void SST_Task_setPrio(SST_Task * const me, SST_TaskPrio prio);

/* SST-PORT critical section */
#define SST_PORT_CRIT_STAT
#define SST_PORT_CRIT_ENTRY()           \
    do                                  \
    {                                   \
        extern uint8_t g_crit_counter;  \
        INTCON2bits.GIE = 0;            \
        g_crit_counter++;               \
    } while (0);

#define SST_PORT_CRIT_EXIT()            \
    do                                  \
    {                                   \
        extern uint8_t g_crit_counter;  \
        g_crit_counter--;               \
        if (g_crit_counter == 0)        \
        {                               \
            INTCON2bits.GIE = 1;        \
        }                               \
    } while (0);

/* SST-PORT pend the Task after posting an event
* NOTE: executed inside SST critical section.
*/
#define SST_PORT_TASK_PEND()  *me->interrupt_pend |= me->interrupt_num

/* the idle SST callback for this SST port */
void SST_onIdle(void);

/* the SST scheduler lock key type */
typedef uint16_t SST_LockKey;

#endif /* SST_PORT_H_ */

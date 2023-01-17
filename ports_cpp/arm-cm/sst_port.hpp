//============================================================================
// Super-Simple Tasker (SST) in C++ port for ARM Cortex-M
//
// Copyright (C) 2006-2023 Quantum Leaps, <state-machine.com>.
//
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//============================================================================
#ifndef SST_PORT_HPP_
#define SST_PORT_HPP_

// additional SST-PORT task attributes for ARM Cortex-M
#define SST_PORT_TASK_ATTR \
    std::uint32_t volatile *m_nvic_pend; \
    std::uint32_t m_nvic_irq;

// additional SST-PORT task operations for ARM Cortex-M
#define SST_PORT_TASK_OPER \
    void activate(void); \
    void setPrio(TaskPrio prio) noexcept; \
    void setIRQ(std::uint32_t irq) noexcept;

// SST-PORT critical section
#define SST_PORT_CRIT_STAT
#define SST_PORT_CRIT_ENTRY() __asm volatile ("cpsid i")
#define SST_PORT_CRIT_EXIT()  __asm volatile ("cpsie i")

// SST-PORT pend the Task after posting an event
// NOTE: executed inside SST critical section.
//
#define SST_PORT_TASK_PEND()  *m_nvic_pend = m_nvic_irq

#endif // SST_PORT_HPP_

//============================================================================
// Super-Simple Tasker (SST0/C++) port
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

#define SST_PORT_MAX_TASK 32U

// additional SST-PORT task attributes for ARM Cortex-M
#define SST_PORT_TASK_ATTR \
    SST::TaskPrio m_prio;

// SST-PORT disabling/enabling interrupts
#define SST_PORT_INT_DISABLE() __asm volatile ("cpsid i")
#define SST_PORT_INT_ENABLE()  __asm volatile ("cpsie i")

// SST-PORT critical section
#define SST_PORT_CRIT_STAT
#define SST_PORT_CRIT_ENTRY() SST_PORT_INT_DISABLE()
#define SST_PORT_CRIT_EXIT()  SST_PORT_INT_ENABLE()

#if (__ARM_ARCH > 6) // ARMv7-M or higher?

// ARMv7-M+ have CLZ instruction for fast LOG2 computations
#if defined __ARMCC_VERSION
    #define SST_LOG2(x_) ((uint_fast8_t)(32U - __builtin_clz((unsigned)(x_))))
#elif defined __GNUC__
    #define SST_LOG2(x_) ((uint_fast8_t)(32U - __builtin_clz((unsigned)(x_))))
#elif defined __ICCARM__
    #define QF_LOG2(n_) ((uint_fast8_t)(32U - __CLZ((unsigned long)(n_))))
#endif /* compiler type */

#endif

namespace SST {
    using ReadySet = std::uint32_t;

    // special idle callback to handle the "idle condition" in SST0
    void onIdleCond(void);
}

#endif // SST_PORT_HPP_

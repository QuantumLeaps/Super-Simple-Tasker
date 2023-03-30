//============================================================================
// Super-Simple Tasker (SST/C++) port to ARM Cortex-M
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
#include "sst.hpp"      // Super-Simple Tasker (SST/C++)
#include "dbc_assert.h" // Design By Contract (DBC) assertions

#define NVIC_PEND    ((uint32_t volatile *)0xE000E200U)
#define NVIC_EN      ((uint32_t volatile *)0xE000E100U)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400U)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14U)
#define SCB_AIRCR   *((uint32_t volatile *)0xE000ED0CU)
#define FPU_FPCCR   *((uint32_t volatile *)0xE000EF34U)

//............................................................................
namespace { // unnamed namespace

DBC_MODULE_NAME("sst_port") // for DBC assertions in this module

// # of unused interrupt priority bits in NVIC
static std::uint32_t nvic_prio_shift;

} // unnamed namespace

namespace SST {

// SST kernel facilities -----------------------------------------------------
void init(void) {
    // determine number of NVIC priority bits by writing 0xFF to the
    // NIVIC IP register for PendSV and then reading back the result,
    // which has only the implemented bits set.
    std::uint32_t tmp = SCB_SYSPRI[3];
    SCB_SYSPRI[3] |= (0xFFU << 16U); // write 0xFF to PendSV prio
    uint32_t prio = ((SCB_SYSPRI[3] >> 16U) & 0xFFU); // read back
    SCB_SYSPRI[3] = tmp; // restore the original PendSV prio

    for (tmp = 0U; tmp < 8U; ++tmp) {
        if ((prio & (1U << tmp)) != 0U) {
            break;
        }
    }
    nvic_prio_shift = tmp;

#if (__ARM_FP != 0)
    // configure the FPU for SST
    FPU_FPCCR |= (1U << 30U)    // automatic FPU state preservation (ASPEN)
                 | (1U << 31U); // lazy stacking (LSPEN)
#endif
}
//............................................................................
void start(void) {
    // Set the NVIC priority grouping to default 0
    //
    // NOTE:
    // Typically the SST port to ARM Cortex-M should waste no NVIC priority
    // bits for grouping. This code ensures this setting, but priority
    // grouping can be still overridden in the application-specific
    // callback SST_onStart().
    //
    std::uint32_t tmp = SCB_AIRCR;
    // clear the key bits 31:16 and priority grouping bits 10:8
    tmp &= ~((0xFFFFU << 16U) | (0x7U << 8U));
    SCB_AIRCR = (0x05FAU << 16U) | tmp;
}

// SST Task facilities -------------------------------------------------------
void Task::setPrio(TaskPrio prio) noexcept {
    //! @pre
    //! - the IRQ number must be already set
    //! - the priority must fit in the NVIC
    DBC_REQUIRE(200,
                (m_nvic_irq != 0U)
                && (prio <= (0xFFU >> nvic_prio_shift)));

    // convert the SST direct priority (1,2,..) to NVIC priority...
    std::uint32_t irq_prio = ((0xFFU >> nvic_prio_shift) + 1U - prio)
                              << nvic_prio_shift;

    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    // set the Task priority of the associated IRQ
    uint32_t tmp = NVIC_IP[m_nvic_irq >> 2U];
    tmp &= ~(0xFFU << ((m_nvic_irq & 3U) << 3U));
    tmp |= (irq_prio << ((m_nvic_irq & 3U) << 3U));
    NVIC_IP[m_nvic_irq >> 2U] = tmp;

    // enable the IRQ associated with the Task
    NVIC_EN[m_nvic_irq >> 5U] = (1U << (m_nvic_irq & 0x1FU));
    SST_PORT_CRIT_EXIT();

    // store the address of NVIC_PEND address and the IRQ bit
    m_nvic_pend = &NVIC_PEND[m_nvic_irq >> 5U];
    m_nvic_irq  = (1U << (m_nvic_irq & 0x1FU));
}
//............................................................................
void Task::activate(void) {
    //! @pre the queue must have some events
    DBC_REQUIRE(300, m_nUsed > 0U);

    // get the event out of the queue
    // NOTE: no critical section because me->tail is accessed only
    // from this task
    Evt const *e = m_qBuf[m_tail];
    if (m_tail == 0U) { // need to wrap the tail?
        m_tail = m_end; // wrap around
    }
    else {
        --m_tail;
    }
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    // some events still present in the queue?
    if ((--m_nUsed) > 0U) {
        *m_nvic_pend = m_nvic_irq; // <=== pend the associated IRQ
    }
    SST_PORT_CRIT_EXIT();

    // dispatch the received event to this task
    dispatch(e); // virtual call
    // TBD: implement event recycling
}
//............................................................................
void SST::Task::setIRQ(std::uint32_t irq) noexcept {
    m_nvic_irq = irq;
}

} // namespace SST

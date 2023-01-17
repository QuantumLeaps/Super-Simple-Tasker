//============================================================================
// Super-Simple Tasker (SST) in C++
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
#include "sst.hpp"   // Super-Simple Tasker (SST) in C++
#include "qassert.h" // assertions for embedded systems

//............................................................................
namespace { // unnamed namespace

Q_DEFINE_THIS_MODULE("sst")

} // unnamed namespace

namespace SST {

// SST kernel facilities -----------------------------------------------------
//............................................................................
int run(void) {
    onStart(); // configure and start the interrupts

    for (;;) { // idle loop of the SST kernel
        onIdle();
    }
}

// SST Task facilities -------------------------------------------------------
void Task::start(
    TaskPrio prio,
    Evt const **qBuf, QCtr qLen,
    Evt const * const ie)
{
    //! @pre
    //! - the queue storage and length must be provided
    //! - the IRQ number must be already set
    Q_REQUIRE_ID(300,
        (qBuf != nullptr) && (qLen > 0U)
        && (m_nvic_irq != 0U));

    m_qBuf  = qBuf;
    m_end   = qLen - 1U;
    m_head  = 0U;
    m_tail  = 0U;
    m_nUsed = 0U;

    setPrio(prio);

    // initialize this task with the initialization event
    init(ie); // virtual call
    // TBD: implement event recycling
}
//............................................................................
void Task::post(Evt const * const e) noexcept {
    //! @pre the queue must be sized adequately and cannot overflow
    Q_REQUIRE_ID(400, m_nUsed <= m_end);

    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    m_qBuf[m_head] = e; // insert event into the queue
    // need to wrap the head?
    if (m_head == 0U) {
        m_head = m_end; // wrap around
    }
    else {
        --m_head;
    }
    ++m_nUsed;
    SST_PORT_CRIT_EXIT();

    *m_nvic_pend = m_nvic_irq; // <=== pend the associated IRQ
}

} // namespace SST

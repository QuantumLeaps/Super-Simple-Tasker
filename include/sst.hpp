//============================================================================
// Super-Simple Tasker (SST/C++)
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
#ifndef SST_HPP_
#define SST_HPP_

#include <cstdint>      // standard integer types
#include "sst_port.hpp" // SST port for specific CPU

namespace SST {

// SST Event facilities ------------------------------------------------------
//! signal of SST event
using Signal = std::uint16_t;

struct Evt {
    Signal sig;
};

// template for downcasting SST events to specific Evt "subclasses"
template<typename EVT_>
EVT_ const *evt_downcast(Evt const *e) {
    return reinterpret_cast<EVT_ const *>(e);
}

// SST Task facilities -------------------------------------------------------

//! SST Task priority
using TaskPrio = std::uint8_t;

//! SST internal event-queue counter
using QCtr = std::uint8_t;

//! SST Task (a.k.a. "Active Object")
class Task {
private:
    Evt const **m_qBuf; //!< ring buffer for the queue
    QCtr m_end;   //!< last index in the ring buffer
    QCtr m_head;  //!< index for inserting events
    QCtr m_tail;  //!< index for removing events
    QCtr m_nUsed; //!< # used entries currently in the queue

#ifdef SST_PORT_TASK_ATTR
    SST_PORT_TASK_ATTR
#endif

public:
    void start(
        TaskPrio prio,
        Evt const **qBuf, QCtr qLen,
        Evt const * const ie);

    void post(Evt const * const e) noexcept;

    virtual void init(Evt const * const ie) = 0;
    virtual void dispatch(Evt const * const e) = 0;

    static int run(void);

#ifdef SST_PORT_TASK_OPER
    // additional Task operations needed by the specific SST port
    SST_PORT_TASK_OPER
#endif
};

// SST Kernel facilities -----------------------------------------------------
void init(void);
void onStart(void);
void onIdle(void);

#ifndef SST_LOG2
inline uint_fast8_t SST_LOG2(std::uint32_t x) {
    static std::uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    std::uint_fast8_t n = 0U;
    SST::ReadySet tmp;

    #if (SST_PORT_MAX_TASK > 16U)
    tmp = static_cast<std::uint32_t>(x >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x = tmp;
    }
    #endif
    #if (SST_PORT_MAX_TASK > 8U)
    tmp = (x >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x = tmp;
    }
    #endif
    tmp = (x >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x = tmp;
    }
    return n + log2LUT[x];
}
#endif

} // namespace SST

// general convenience utilities ---------------------------------------------
#ifndef ARRAY_NELEM
//! convenience macro to provide the number of elements in the array a_
#define ARRAY_NELEM(a_)  (sizeof(a_) / sizeof((a_)[0]))
#endif /* ARRAY_NELEM */

#endif // SST_HPP_

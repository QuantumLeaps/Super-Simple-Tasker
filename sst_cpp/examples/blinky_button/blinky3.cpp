//============================================================================
// Super-Simple Tasker (SST/C++) Example
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
#include "sst.hpp"           // SST framework
#include "bsp.hpp"           // Board Support Package interface
#include "blinky_button.hpp" // application shared interface

namespace {

Q_DEFINE_THIS_FILE

} // unnamed namespace

namespace App {

//............................................................................
class Blinky3 : public SST::Task {
    std::uint16_t m_toggles;
    std::uint8_t m_ticks;
    std::uint8_t m_tick_ctr;

public:
    static Blinky3 inst;
    void init(SST::Evt const * const ie) override;
    void dispatch(SST::Evt const * const e) override;
};

//............................................................................
Blinky3 Blinky3::inst; // the Blinky3 instance
SST::Task * const AO_Blinky3 = &Blinky3::inst; // opaque AO pointer

//............................................................................
void Blinky3::init(SST::Evt const * const ie) {
    // the initial event must be provided and must be WORKLOAD_SIG
    Q_REQUIRE((ie != nullptr) && (ie->sig == BLINKY_WORK_SIG));

    m_toggles = SST::evt_downcast<BlinkyWorkEvt>(ie)->toggles;
    m_ticks = SST::evt_downcast<BlinkyWorkEvt>(ie)->ticks;
    m_tick_ctr = m_ticks;
}
//............................................................................
void Blinky3::dispatch(SST::Evt const * const e) {
    switch (e->sig) {
        case TICK_SIG: {
            --m_tick_ctr;
            if (m_tick_ctr == 0U) {
                m_tick_ctr = m_ticks;
                for (uint16_t i = m_toggles; i > 0U; --i) {
                    BSP::d2on();
                    BSP::d2off();
                }
            }
            break;
        }
        case BLINKY_WORK_SIG: {
            BSP::d2on();
            m_toggles = SST::evt_downcast<BlinkyWorkEvt>(e)->toggles;
            m_ticks = SST::evt_downcast<BlinkyWorkEvt>(e)->ticks;
            BSP::d2off();
            break;
        }
        default: {
            Q_ERROR(); // unexpected event
            break;
        }
    }
}

} // namespace App

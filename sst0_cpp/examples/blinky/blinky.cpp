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
#include "blinky.hpp"        // application shared interface

namespace {

DBC_MODULE_NAME("blinky")   // for DBC assertions in this module */

} // unnamed namespace

namespace App {

//............................................................................
class Blinky : public SST::Task {
    SST::TimeEvt m_te1;
    SST::TimeEvt m_te2;

public:
    Blinky(void);
    void init(SST::Evt const * const ie) override;
    void dispatch(SST::Evt const * const e) override;
    static Blinky inst;
};

//............................................................................
Blinky Blinky::inst; // the Blinky instance
SST::Task * const AO_Blinky = &Blinky::inst; // opaque AO pointer

//............................................................................
Blinky::Blinky(void)
  : m_te1(TIMEOUT1_SIG, this),
    m_te2(TIMEOUT2_SIG, this)
{}
//............................................................................
void Blinky::init(SST::Evt const * const ie) {
    static_cast<void>(ie); // unused parameter

    m_te2.arm(1U, 0U);
}
//............................................................................
void Blinky::dispatch(SST::Evt const * const e) {
    switch (e->sig) {
        case TIMEOUT1_SIG: {
            BSP::ledOff();
            m_te2.arm(BSP::TICKS_PER_SEC*3U/4U, 0U);
            break;
        }
        case TIMEOUT2_SIG: {
            BSP::ledOn();
            m_te1.arm(BSP::TICKS_PER_SEC/4U, 0U);
            break;
        }
        default: {
            DBC_ERROR(500); // unexpected event
            break;
        }
    }
}

} // namespace App

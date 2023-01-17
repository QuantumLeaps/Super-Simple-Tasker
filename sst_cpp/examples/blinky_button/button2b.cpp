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
class Button2b : public SST::Task {
    // add internal variables for this AO...

public:
    static Button2b inst;
    void init(SST::Evt const * const ie) override;
    void dispatch(SST::Evt const * const e) override;
};

//............................................................................
Button2b Button2b::inst; // the Button2b instance
SST::Task * const AO_Button2b = &Button2b::inst; // opaque AO pointer

//............................................................................
void Button2b::init(SST::Evt const * const /*ie*/) {
}
//............................................................................
void Button2b::dispatch(SST::Evt const * const e) {
    switch (e->sig) {
        case FORWARD_PRESSED_SIG: {
            BSP::d3on();
            // Button2b --> Blinky3
            AO_Blinky3->post(BSP::getWorkEvtBlinky3(1U));
            BSP::d3off();

            for (uint16_t i = SST::evt_downcast<ButtonWorkEvt>(e)->toggles;
                 i > 0U; --i)
            {
                BSP::d3on();
                BSP::d3off();
            }
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP::d3on();
            // Button2b --> Blinky3
            AO_Blinky3->post(BSP::getWorkEvtBlinky3(0U));
            BSP::d3off();

            for (uint16_t i = SST::evt_downcast<ButtonWorkEvt>(e)->toggles;
                 i > 0U; --i)
            {
                BSP::d3on();
                BSP::d3off();
            }
            break;
        }
        default: {
            Q_ERROR(); // unexpected event
            break;
        }
    }
}

} // namespace App

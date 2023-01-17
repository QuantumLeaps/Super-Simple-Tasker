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
class Button2a : public SST::Task {
    // add internal variables for this AO...

public:
    static Button2a inst;
    void init(SST::Evt const * const ie) override;
    void dispatch(SST::Evt const * const e) override;
};

//............................................................................
Button2a Button2a::inst; // the Button2a instance
SST::Task * const AO_Button2a = &Button2a::inst; // opaque AO pointer

//............................................................................
void Button2a::init(SST::Evt const * const /*ie*/) {
}
//............................................................................
void Button2a::dispatch(SST::Evt const * const e) {
    switch (e->sig) {
        case BUTTON_PRESSED_SIG: {
            BSP::d4on();
            // Button2a --> Blinky1
            AO_Blinky1->post(BSP::getWorkEvtBlinky1(1U));
            BSP::d4off();

            for (uint16_t i = SST::evt_downcast<ButtonWorkEvt>(e)->toggles;
                 i > 0U; --i)
            {
                BSP::d4on();
                BSP::d4off();
            }
            break;
        }
        case FORWARD_PRESSED_SIG: {
            BSP::d4on();
            // immutable event can be forwarded to another Task
            AO_Button2b->post(e); // Button2a --> Button2b
            BSP::d4off();
            break;
        }
        case BUTTON_RELEASED_SIG: {
            static BlinkyWorkEvt const bw2evt = {
                { BLINKY_WORK_SIG }, 30U, 7U
            };
            AO_Blinky1->post(&bw2evt.super); // Button2b --> Blinky1

            for (uint16_t i = SST::evt_downcast<ButtonWorkEvt>(e)->toggles;
                 i > 0U; --i)
            {
                BSP::d4on();
                BSP::d4off();
            }
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP::d4on();
            // immutable event can be forwarded to another Task
            AO_Button2b->post(e); // Button2a --> Button2b
            BSP::d4off();
            break;
        }
        default: {
            Q_ERROR(); // unexpected event
            break;
        }
    }
}

} // namespace App

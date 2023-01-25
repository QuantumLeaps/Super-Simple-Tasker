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
#ifndef BLINKY_BUTTON_HPP_
#define BLINKY_BUTTON_HPP_

#include "dbc_assert.h" /* Design By Contract (DBC) assertions */

namespace App {

enum Signals {
    TICK_SIG,
    BUTTON_PRESSED_SIG,
    BUTTON_RELEASED_SIG,
    BLINKY_WORK_SIG,
    FORWARD_PRESSED_SIG,
    FORWARD_RELEASED_SIG,
    // ...
    MAX_SIG  // the last signal
};

// event with parameters
struct BlinkyWorkEvt {
    SST::Evt super;
    std::uint16_t toggles; // number of toggles of the signal
    std::uint8_t ticks;    // number of clock ticks between
};

// event with parameters
struct ButtonWorkEvt {
    SST::Evt super;
    std::uint16_t toggles; // number of toggles of the signal
};

extern SST::Task * const AO_Blinky1;  // opaque active object pointer
extern SST::Task * const AO_Blinky3;  // opaque active object pointer
extern SST::Task * const AO_Button2a; // opaque active object pointer
extern SST::Task * const AO_Button2b; // opaque active object pointer

} // namespace App

#endif // BLINKY_BUTTON_HPP_

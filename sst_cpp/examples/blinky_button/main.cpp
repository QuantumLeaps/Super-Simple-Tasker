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

//............................................................................
int main() {
    SST::init(); // initialize the SST kernel
    BSP::init(); // initialize the Board Support Package

    // instantiate and start all SST tasks...
    static SST::Evt const *blinky1QSto[10]; // Event queue storage
    App::AO_Blinky1->start(
        1U,           // SST-priority
        blinky1QSto,  // storage for the AO's queue
        ARRAY_NELEM(blinky1QSto), // queue length
        BSP::getWorkEvtBlinky1(0U)); // initialization event

    static SST::Evt const *button2aQSto[8]; // Event queue storage
    App::AO_Button2a->start(
        2U,           // SST-priority
        button2aQSto, // storage for the AO's queue
        ARRAY_NELEM(button2aQSto), // queue length
        nullptr);     // initialization event

    static SST::Evt const *button2bQSto[6]; // Event queue storage
    App::AO_Button2b->start(
        2U,           // SST-priority
        button2bQSto, // storage for the AO's queue
        ARRAY_NELEM(button2bQSto), // queue length
        nullptr);     // initialization event

    static SST::Evt const *blinky3QSto[4]; // Event queue storage
    App::AO_Blinky3->start(
        3U,           // SST-priority
        blinky3QSto,  // storage for the AO's queue
        ARRAY_NELEM(blinky3QSto), // queue length
        BSP::getWorkEvtBlinky3(0U)); // initialization event

    return SST::Task::run(); // run the SST tasks
    // NOTE: in embedded systems SST::Task::run() should not return
}


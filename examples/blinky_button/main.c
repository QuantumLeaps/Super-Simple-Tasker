/*============================================================================
* Super-Simple Tasker (SST) Example
*
* Copyright (C) 2006-2023 Quantum Leaps, <state-machine.com>.
*
* SPDX-License-Identifier: MIT
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
============================================================================*/
#include "sst.h"           /* SST framewrok */
#include "bsp.h"           /* Board Support Package interface */
#include "blinky_button.h" /* application shared interface */

/*..........................................................................*/
int main() {
    SST_init(); /* initialize the SST kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* instantiate and start all SST tasks... */
    Blinky1_ctor();
    static SST_Evt const *blinky1QSto[10]; /* Event queue storage */
    SST_Task_start(
        AO_Blinky1,     /* AO pointer to start */
        1U,             /* SST-priority */
        blinky1QSto,    /* storage for the AO's queue */
        Q_DIM(blinky1QSto),   /* queue length */
        getInitEvtBlinky1()); /* initialization event */

    Button2a_ctor();
    static SST_Evt const *button2aQSto[8]; /* Event queue storage */
    SST_Task_start(
        AO_Button2a,    /* AO pointer to start */
        2U,             /* SST-priority */
        button2aQSto,   /* storage for the AO's queue */
        Q_DIM(button2aQSto), /* queue length */
        (SST_Evt const *)0); /* initialization event -- not used */

    Button2b_ctor();
    static SST_Evt const *button2bQSto[6]; /* Event queue storage */
    SST_Task_start(
        AO_Button2b,    /* AO pointer to start */
        2U,             /* SST-priority */
        button2bQSto,   /* storage for the AO's queue */
        Q_DIM(button2bQSto), /* queue length */
        (SST_Evt const *)0); /* initialization event -- not used */

    Blinky3_ctor();
    static SST_Evt const *blinky3QSto[4]; /* Event queue storage */
    SST_Task_start(
        AO_Blinky3,     /* AO pointer to start */
        3U,             /* SST-priority */
        blinky3QSto,    /* storage for the AO's queue */
        Q_DIM(blinky3QSto),   /* queue length */
        getInitEvtBlinky3()); /* initialization event */

    return SST_run(); /* run the SST kernel */
}


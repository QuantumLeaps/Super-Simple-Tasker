/*****************************************************************************
* Product: SST example, 80x86, Turbo C++ 1.01
*
* Copyright (C) 2006 Miro Samek and Robert Ward. All rights reserved.
*
* MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*****************************************************************************/
#include "sst_port.h"
#include "sst_exa.h"
#include "bsp.h"

/*..........................................................................*/
void kbdTask(SSTEvent e) {
    static uint32_t kbdTaskCtr;

    Video_printNumAt(22, 19 - KBD_TASK_PRIO, VIDEO_FGND_YELLOW, ++kbdTaskCtr);
    busyDelay();                                     /* for testing, NOTE01 */

    switch (e.sig) {
        case INIT_SIG: {
            Video_printStrAt( 1, 19 - KBD_TASK_PRIO,
                             VIDEO_FGND_WHITE, "kbdTask");
            break;
        }
        case KBD_SIG: {
            if (e.par == (SSTParam)0x81) {          /* is this the ESC key? */
                SST_exit();
            }
            else if ((e.par & 1) != 0) {      /* pick one of the Tick Tasks */
                SST_post(TICK_TASK_A_PRIO,     /* no synchronous preemption */
                         COLOR_SIG, e.par & 0xF);
            }
            else {
                SST_post(TICK_TASK_B_PRIO,        /* synchronous preemption */
                         COLOR_SIG, (e.par & 0xF));
            }
            break;
        }
    }
}

/*****************************************************************************
* NOTE01:
* The call to busyDelay() is added only to extend the execution time
* to increase the chance of an "asynchronous" preemption.
*/

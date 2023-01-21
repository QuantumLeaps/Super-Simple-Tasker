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

#include <stdlib.h>                                         /* for random() */

/*..........................................................................*/
void tickTaskA(SSTEvent e) {
    static uint32_t tickTaskACtr;
    static uint8_t colorA = VIDEO_BGND_LIGHT_GRAY;

    Video_printNumAt(22, 19 - TICK_TASK_A_PRIO,
                     VIDEO_FGND_YELLOW, ++tickTaskACtr);
    busyDelay();                                     /* for testing, NOTE01 */

    switch (e.sig) {
        case INIT_SIG: {
            Video_printStrAt( 1, 19 - TICK_TASK_A_PRIO,
                              VIDEO_FGND_WHITE, "tickTaskA");
            break;
        }
        case TICK_SIG: {
            uint8_t x, y;
            uint8_t mutex;

            mutex = SST_mutexLock(TICK_TASK_B_PRIO); /* the other tick task */
            x = random(34);
            y = random(13);
            SST_mutexUnlock(mutex);

            Video_printChAt(x + 43, y + 8, colorA, 'A');
            break;
        }
        case COLOR_SIG: {
            colorA = e.par;          /* color is delivered in the parameter */
            break;
        }
    }
}
/*..........................................................................*/
void tickTaskB(SSTEvent e) {
    static uint32_t tickTaskBCtr;
    static uint8_t colorB = VIDEO_BGND_LIGHT_GRAY;

    Video_printNumAt(22, 19 - TICK_TASK_B_PRIO,
                     VIDEO_FGND_YELLOW, ++tickTaskBCtr);
    busyDelay();                                     /* for testing, NOTE01 */

    switch (e.sig) {
        case INIT_SIG: {
            Video_printStrAt( 1, 19 - TICK_TASK_B_PRIO,
                              VIDEO_FGND_WHITE, "tickTaskB");
            break;
        }
        case TICK_SIG: {
            uint8_t x, y;
            uint8_t mutex;

            mutex = SST_mutexLock(TICK_TASK_A_PRIO); /* the other tick task */
            x = random(34);
            y = random(13);
            SST_mutexUnlock(mutex);

            Video_printChAt(x + 43, y + 8, colorB, 'B');
            break;
        }
        case COLOR_SIG: {
            colorB = e.par;          /* color is delivered in the parameter */
            break;
        }
    }
}

/*****************************************************************************
* NOTE01:
* The call to busyDelay() is added only to extend the execution time
* to increase the chance of an "asynchronous" preemption.
*/

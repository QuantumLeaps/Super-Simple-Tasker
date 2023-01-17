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
#ifndef sst_exa_h
#define sst_exa_h

void tickTaskA(SSTEvent e);
void tickTaskB(SSTEvent e);
void kbdTask(SSTEvent e);

enum Events {                         /* the events used in the application */
    INIT_SIG,                                       /* initialization event */
    TICK_SIG,
    KBD_SIG,
    COLOR_SIG
};

enum SSTPriorities {     /* the SST priorities don't need to be consecutive */
    /* task priorities... */
    TICK_TASK_A_PRIO = 2,
    KBD_TASK_PRIO    = 5,
    TICK_TASK_B_PRIO = 7,

    /* ISR priorities... */
    KBD_ISR_PRIO     = 0xFF - 1,
    TICK_ISR_PRIO    = 0xFF
};

#endif                                                         /* sst_exa_h */


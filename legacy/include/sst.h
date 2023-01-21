/*****************************************************************************
* SST platform-independent public interface
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
#ifndef sst_h
#define sst_h

#include <stdint.h>                 /* exact-width integer types, ANSI C'99 */

typedef uint8_t SSTSignal;
typedef uint8_t SSTParam;

typedef struct SSTEventTag SSTEvent;
struct SSTEventTag {
    SSTSignal sig;
    SSTParam  par;
};

typedef void (*SSTTask)(SSTEvent e);

void SST_init(void);
void SST_task(SSTTask task, uint8_t prio, SSTEvent *queue, uint8_t qlen,
              SSTSignal sig, SSTParam  par);
void SST_start(void);
void SST_run(void);
void SST_onIdle(void);
void SST_exit(void);

uint8_t SST_post(uint8_t prio, SSTSignal sig, SSTParam  par);

uint8_t SST_mutexLock(uint8_t prioCeiling);
void SST_mutexUnlock(uint8_t orgPrio);

void SST_schedule_(void);

                                            /* SST interrupt entry and exit */
#define SST_ISR_ENTRY(pin_, isrPrio_) do { \
    (pin_) = SST_currPrio_; \
    SST_currPrio_ = (isrPrio_); \
    SST_INT_UNLOCK(); \
} while (0)

#define SST_ISR_EXIT(pin_, EOI_command_) do { \
    SST_INT_LOCK(); \
    (EOI_command_); \
    SST_currPrio_ = (pin_); \
    SST_schedule_(); \
} while (0)


/* public-scope objects */
extern uint8_t SST_currPrio_;     /* current priority of the executing task */
extern uint8_t SST_readySet_;                              /* SST ready-set */

#endif                                                             /* sst_h */

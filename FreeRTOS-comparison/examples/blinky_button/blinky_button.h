/*============================================================================
* Super-Simple Tasker (SST/C) Example
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
#ifndef BLINKY_BUTTON_H_
#define BLINKY_BUTTON_H_

/*--------------------------------------------------------------------------*/
/* Event facilities... */
/*! Event signal */
typedef uint16_t Signal;

/*! Event class */
typedef struct {
    Signal sig;
} Event;

/*! macro for downcasting Events to specific Event "subclasses" */
#define EVT_DOWNCAST(EVT_, e_) ((EVT_ const *)(e_))

/*--------------------------------------------------------------------------*/
/* Custom Events for this application... */

enum Signals {
    TICK_SIG,
    BUTTON_PRESSED_SIG,
    BUTTON_RELEASED_SIG,
    BLINKY_WORK_SIG,
    FORWARD_PRESSED_SIG,
    FORWARD_RELEASED_SIG,
    /* ... */
    MAX_SIG  /* the last signal */
};

typedef struct {
    Event super;      /* inherit Event */
    uint16_t toggles; /* number of toggles of the signal */
    uint8_t ticks;    /* number of clock ticks between */
} BlinkyWorkEvt;

typedef struct {
    Event super;      /* inherit Event */
    uint16_t toggles; /* number of toggles of the signal */
} ButtonWorkEvt;

/*--------------------------------------------------------------------------*/
/* Tasks and event queues for this application... */

void Blinky1_task(void *pvParameters);
extern void * const AO_Blinky1;  /* opaque active object pointer */
extern QueueHandle_t QUEUE_Blinky1; /* queue handle */

void Blinky3_task(void *pvParameters);
extern void * const AO_Blinky3;  /* opaque active object pointer */
extern QueueHandle_t QUEUE_Blinky3; /* queue handle */

void Button2a_task(void *pvParameters);
extern void * const AO_Button2a;  /* opaque active object pointer */
extern QueueHandle_t QUEUE_Button2a; /* queue handle */

void Button2b_task(void *pvParameters);
extern void * const AO_Button2b;  /* opaque active object pointer */
extern QueueHandle_t QUEUE_Button2b; /* queue handle */

/* general convenience utilities -------------------------------------------*/
#ifndef ARRAY_NELEM
/*! convenience macro to provide the number of elements in the array a_ */
#define ARRAY_NELEM(a_)  (sizeof(a_) / sizeof((a_)[0]))
#endif /* ARRAY_NELEM */

#endif /* BLINKY_BUTTON_H_ */

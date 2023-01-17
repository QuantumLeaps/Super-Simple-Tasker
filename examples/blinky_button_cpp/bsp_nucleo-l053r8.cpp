//============================================================================
// Super-Simple Tasker (SST) in C++ Example
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
#include "sst.hpp"           // SST framewrok
#include "bsp.hpp"           // Board Support Package interface
#include "blinky_button.hpp" // application shared interface

#include "stm32l0xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

// Local-scope defines -------------------------------------------------------

// Test pins on PA
#define PIN_PA0  (1U << 0)
#define PIN_PA1  (1U << 1)
#define PIN_PA4  (1U << 4)
#define PIN_PA5  (1U << 5) // LED LD2-Green on PA.5)
#define PIN_PA6  (1U << 6)
#define PIN_PA7  (1U << 7)

// Button B1 on PC.13
#define BTN_B1   (1U << 13)

// ISRs used in the application ==============================================
extern "C" {

void SysTick_Handler(void) {   // system clock tick ISR
    BSP::d1on();

    // immutable timeout event
    static SST::Evt const tickEvt = { App::TICK_SIG };
    App::AO_Blinky1->post(&tickEvt); // every tick is (too) fast for Blinky1
    App::AO_Blinky3->post(&tickEvt);

    // get state of the user button
    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    //
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current = ~GPIOC->IDR; // read GPIO PortC
    uint32_t tmp = buttons.depressed; // save the debounced depressed
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    if ((tmp & BTN_B1) != 0U) {   // debounced B1 state changed?
        if ((buttons.depressed & BTN_B1) != 0U) { // is B1 depressed?
            // immutable button-press event
            static App::ButtonWorkEvt const pressEvt = {
                { App::BUTTON_PRESSED_SIG }, 60U
            };
            App::AO_Button2a->post(&pressEvt.super);
        }
        else { // B1 is released
            // immutable button-release event
            static App::ButtonWorkEvt const releaseEvt = {
                { App::BUTTON_RELEASED_SIG }, 80U
            };
            App::AO_Button2a->post(&releaseEvt.super);
        }
    }

    BSP::d1off();
}

// Assertion handler =========================================================
void Q_onAssert(char const * const module, int const loc) {
    //
    // NOTE: add here your application-specific error handling
    //
    (void)module;
    (void)loc;

    // set PRIMASK to disable interrupts and stop SST right here
    __asm volatile ("cpsid i");

#ifndef NDEBUG
    for (;;) { // keep blinking LED2
        BSP::d6on();  // turn LED2 on
        uint32_t volatile ctr;
        for (ctr = 10000U; ctr != 0U; --ctr) {
        }
        BSP::d6off(); // turn LED2 off
        for (ctr = 10000U; ctr != 0U; --ctr) {
        }
    }
#endif
    NVIC_SystemReset();
}

// SST task activations ======================================================
// preprocessor switch to choose between regular and reserved IRQs
#define REGULAR_IRQS

#ifdef REGULAR_IRQS
// repurpose regular IRQs for SST Tasks
void PVD_IRQHandler(void)  { App::AO_Blinky3->activate();  }
void RTC_IRQHandler(void)  { App::AO_Button2b->activate(); }
void TSC_IRQHandler(void)  { App::AO_Button2a->activate(); }
void I2C2_IRQHandler(void) { App::AO_Blinky1->activate();  }
#else
void Reserved14_IRQHandler(void); // prototype
void Reserved16_IRQHandler(void); // prototype
void Reserved18_IRQHandler(void); // prototype
void Reserved19_IRQHandler(void); // prototype
void Reserved21_IRQHandler(void); // prototype

// use reserved IRQs for SST Tasks
void Reserved14_IRQHandler(void) { App::AO_Blinky3->activate();  }
void Reserved16_IRQHandler(void) { App::AO_Button2b->activate(); }
void Reserved18_IRQHandler(void) { App::AO_Button2a->activate(); }
void Reserved19_IRQHandler(void) { App::AO_Blinky1->activate();  }
#endif

} // extern "C"

namespace BSP {

// BSP functions =============================================================
void init(void) {
    // assign IRQs to tasks. NOTE: critical for SST...
#ifdef REGULAR_IRQS
    // repurpose regular IRQs for SST Tasks
    App::AO_Blinky3->setIRQ(PVD_IRQn);
    App::AO_Button2b->setIRQ(RTC_IRQn);
    App::AO_Button2a->setIRQ(TSC_IRQn);
    App::AO_Blinky1->setIRQ(I2C2_IRQn);
#else
    // use reserved IRQs for SST Tasks
    App::AO_Blinky3->setIRQ (14U);
    App::AO_Button2b->setIRQ (16U);
    App::AO_Button2a->setIRQ(18U);
    App::AO_Blinky1->setIRQ(19U);
#endif

    // enable GPIO port PA clock
    RCC->IOPENR |= (1U << 0);

    // set all used PA pins as push-pull output, no pull-up, pull-down
    GPIOA->MODER   &= ~((3U << 2*0) | (3U << 2*1) | (3U << 2*4)
                        | (3U << 2*5) | (3U << 2*6) | (3U << 2*7));
    GPIOA->MODER   |=  ((1U << 2*0) | (1U << 2*1) | (1U << 2*4)
                        | (1U << 2*5) | (1U << 2*6) | (1U << 2*7));
    GPIOA->OTYPER  &= ~((1U <<   0) |  (1U <<  1) | (1U <<   4)
                         | (1U <<  5) |  (1U <<  6) | (1U <<   7));
    GPIOA->OSPEEDR &= ~((3U << 2*0) | (3U << 2*1) | (3U << 2*4)
                        | (3U << 2*5) | (3U << 2*6) | (3U << 2*7));
    GPIOA->OSPEEDR |=  ((1U << 2*0) | (1U << 2*1) | (1U << 2*4)
                        | (1U << 2*5) | (1U << 2*6) | (1U << 2*7));
    GPIOA->PUPDR   &= ~((3U << 2*0) | (3U << 2*1) | (3U << 2*4)
                        | (3U << 2*5) | (3U << 2*6) | (3U << 2*7));

    // enable GPIOC clock port for the Button B1
    RCC->IOPENR |=  (1U << 2);

    // configure Button (PC.13) pin as input, no pull-up, pull-down
    GPIOC->MODER   &= ~(3U << 2*13);
    GPIOC->OSPEEDR &= ~(3U << 2*13);
    GPIOC->OSPEEDR |=  (1U << 2*13);
    GPIOC->PUPDR   &= ~(3U << 2*13);
}
//............................................................................
void d1on(void)  { GPIOA->BSRR |= (PIN_PA7);       }
void d1off(void) { GPIOA->BSRR |= (PIN_PA7 << 16); }
//............................................................................
void d2on(void)  { GPIOA->BSRR |= (PIN_PA6);       }
void d2off(void) { GPIOA->BSRR |= (PIN_PA6 << 16); }
//............................................................................
void d3on(void)  { GPIOA->BSRR |= (PIN_PA4);       }
void d3off(void) { GPIOA->BSRR |= (PIN_PA4 << 16); }
//............................................................................
void d4on(void)  { GPIOA->BSRR |= (PIN_PA1);       }
void d4off(void) { GPIOA->BSRR |= (PIN_PA1 << 16); }
//............................................................................
void d5on(void)  { GPIOA->BSRR |= (PIN_PA0);       }
void d5off(void) { GPIOA->BSRR |= (PIN_PA0 << 16); }
//............................................................................
void d6on(void)  { GPIOA->BSRR |= (PIN_PA5);       }
void d6off(void) { GPIOA->BSRR |= (PIN_PA5 << 16); }

} // namespace BSP

namespace App {

//............................................................................
SST::Evt const *getInitEvtBlinky3(void) {
    static BlinkyWorkEvt const initBlinky3 = {
        { BLINKY_WORK_SIG }, 10U, 3U
    };
    return &initBlinky3.super;
}
//............................................................................
SST::Evt const *getInitEvtBlinky1(void) {
    static BlinkyWorkEvt const initBlinky1 = {
        { BLINKY_WORK_SIG }, 30U, 7U,
    };
    return &initBlinky1.super;
}
} // namespace App


// SST callbacks =============================================================
namespace SST {

void onStart(void) {
    SystemCoreClockUpdate();

    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config((SystemCoreClock / BSP::TICKS_PER_SEC) + 1U);

    // set priorities of ISRs used in the system
    NVIC_SetPriority(SysTick_IRQn, 0U);
    // ...
}
//............................................................................
void onIdle(void) {
#ifdef NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    __WFI(); // Wait-For-Interrupt
#else
    BSP::d6on();  // turn LED2 on
    BSP::d6off(); // turn LED2 off
#endif
}

} // namespace SST

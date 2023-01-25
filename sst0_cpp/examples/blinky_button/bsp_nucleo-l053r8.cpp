//============================================================================
// Super-Simple Tasker (SST0/C++) Example for STM32 NUCLEO-L053R8
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

#include "stm32l0xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

// Local-scope defines -------------------------------------------------------
namespace {

DBC_MODULE_NAME("bsp_nucleo-l053r8") /* for DBC assertions in this module */

} // unnamed workspace

// test pins on GPIO PA
#define TST1_PIN  7U
#define TST2_PIN  6U
#define TST3_PIN  4U
#define TST4_PIN  1U
#define TST5_PIN  0U
#define TST6_PIN  5U /* LED LD2-Green */

// buttons on GPIO PC
#define B1_PIN    13U

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
    if ((tmp & (1U << B1_PIN)) != 0U) { // debounced B1 state changed?
        if ((buttons.depressed & (1U << B1_PIN)) != 0U) { // depressed?
            // immutable button-press event
            static App::ButtonWorkEvt const pressEvt = {
                { App::BUTTON_PRESSED_SIG }, 60U
            };
            // immutable forward-press event
            static App::ButtonWorkEvt const fPressEvt = {
                { App::FORWARD_PRESSED_SIG }, 60U
            };
            App::AO_Button2a->post(&fPressEvt.super);
            App::AO_Button2a->post(&pressEvt.super);
        }
        else { // B1 is released
            // immutable button-release event
            static App::ButtonWorkEvt const releaseEvt = {
                { App::BUTTON_RELEASED_SIG }, 80U
            };
            // immutable forward-release event
            static App::ButtonWorkEvt const fReleaseEvt = {
                { App::FORWARD_RELEASED_SIG }, 80U
            };
            App::AO_Button2a->post(&fReleaseEvt.super);
            App::AO_Button2a->post(&releaseEvt.super);
        }
    }

    BSP::d1off();
}

// Assertion handler =========================================================
void DBC_fault_handler(char const * const module, int const label) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)label;

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

} // extern "C"

namespace BSP {

// BSP functions =============================================================
void init(void) {

    // enable GPIO port PA clock
    RCC->IOPENR |= (1U << 0U);

    // set all used GPIOA pins as push-pull output, no pull-up, pull-down
    GPIOA->MODER &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));
    GPIOA->MODER |=
         ((1U << 2U*TST1_PIN) | (1U << 2U*TST2_PIN) | (1U << 2U*TST3_PIN) |
          (1U << 2U*TST4_PIN) | (1U << 2U*TST5_PIN) | (1U << 2U*TST6_PIN));
    GPIOA->OTYPER &=
        ~((1U <<    TST1_PIN) | (1U <<    TST2_PIN) | (1U <<    TST3_PIN) |
          (1U <<    TST4_PIN) | (1U <<    TST5_PIN) | (1U <<    TST6_PIN));
    GPIOA->PUPDR &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));

    // enable GPIOC clock port for the Button B1
    RCC->IOPENR |=  (1U << 2U);

    // configure Button B1 pin on GPIOC as input, no pull-up, pull-down
    GPIOC->MODER &= ~(3U << 2U*B1_PIN);
    GPIOC->PUPDR &= ~(3U << 2U*B1_PIN);
}
//............................................................................
void d1on(void)  { GPIOA->BSRR = (1U << TST1_PIN);         }
void d1off(void) { GPIOA->BSRR = (1U << (TST1_PIN + 16U)); }
//............................................................................
void d2on(void)  { GPIOA->BSRR = (1U << TST2_PIN);         }
void d2off(void) { GPIOA->BSRR = (1U << (TST2_PIN + 16U)); }
//............................................................................
void d3on(void)  { GPIOA->BSRR = (1U << TST3_PIN);         }
void d3off(void) { GPIOA->BSRR = (1U << (TST3_PIN + 16U)); }
//............................................................................
void d4on(void)  { GPIOA->BSRR = (1U << TST4_PIN);         }
void d4off(void) { GPIOA->BSRR = (1U << (TST4_PIN + 16U)); }
//............................................................................
void d5on(void)  { GPIOA->BSRR = (1U << TST5_PIN);         }
void d5off(void) { GPIOA->BSRR = (1U << (TST5_PIN + 16U)); }
//............................................................................
void d6on(void)  { GPIOA->BSRR = (1U << TST6_PIN);         } // LED2
void d6off(void) { GPIOA->BSRR = (1U << (TST6_PIN + 16U)); }

//............................................................................
SST::Evt const *getWorkEvtBlinky1(uint8_t num) {
    // immutable work events for Blinky1
    static App::BlinkyWorkEvt const workBliny1[] = {
        { { App::BLINKY_WORK_SIG }, 40U, 5U },
        { { App::BLINKY_WORK_SIG }, 30U, 7U }
    };
    DBC_REQUIRE(500, num < ARRAY_NELEM(workBliny1)); // num must be in range
    return &workBliny1[num].super;
}
//............................................................................
SST::Evt const *getWorkEvtBlinky3(uint8_t num) {
    /* immutable work events for Blinky3 */
    static App::BlinkyWorkEvt const workBlinky3[] = {
        { { App::BLINKY_WORK_SIG }, 20U, 5U },
        { { App::BLINKY_WORK_SIG }, 10U, 3U   }
    };
    DBC_REQUIRE(600, num < ARRAY_NELEM(workBlinky3)); // num must be in range
    return &workBlinky3[num].super;
}

} // namespace BSP

// SST callbacks =============================================================
namespace SST {

void onStart(void) {
    SystemCoreClockUpdate();

    // set up the SysTick timer to fire at BSP::TICKS_PER_SEC rate
    SysTick_Config((SystemCoreClock / BSP::TICKS_PER_SEC) + 1U);

    // set priorities of ISRs used in the system
    NVIC_SetPriority(SysTick_IRQn, 0U);
    // ...
}
//............................................................................
void onIdleCond(void) { // NOTE: called with interrupts DISABLED
    BSP::d6on();  // turn LED2 on
#ifdef NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    BSP::d6off(); // turn LED2 off
    __WFI(); // Wait-For-Interrupt
    BSP::d6on();  // turn LED2 on
#endif
    BSP::d6off(); // turn LED2 off
    SST_PORT_INT_ENABLE(); // NOTE: enable interrupts for SS0
}

} // namespace SST


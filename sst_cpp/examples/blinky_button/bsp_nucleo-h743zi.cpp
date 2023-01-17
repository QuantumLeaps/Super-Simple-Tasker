//============================================================================
// Super-Simple Tasker (SST/C++) Example for STM32 NUCLEO-H74cZI
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

#include "stm32h743xx.h"  // CMSIS-compliant header file for the MCU used
#include <math.h>         // to exercise the FPU
// add other drivers if necessary...

// Local-scope defines -------------------------------------------------------
namespace {

Q_DEFINE_THIS_FILE // for assertions in this file

}

// test pins on GPIO PB
#define TST1_PIN  0U  /* PB.0  LED1-Green */
#define TST2_PIN  14U /* PB.14 LED3-Red   */
#define TST3_PIN  4U
#define TST4_PIN  5U
#define TST5_PIN  6U
#define TST6_PIN  7U  /* PB.7  LED2-Blue  */

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
    uint32_t current = GPIOC->IDR; // read GPIO PortC
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
//#define REGULAR_IRQS

#ifdef REGULAR_IRQS
// repurpose regular IRQs for SST Tasks
void OTG_FS_EP1_OUT_IRQHandler(void) { App::AO_Blinky3->activate();  }
void OTG_FS_EP1_IN_IRQHandler(void)  { App::AO_Button2b->activate(); }
void OTG_FS_WKUP_IRQHandler(void)    { App::AO_Button2a->activate(); }
void OTG_FS_IRQHandler(void)         { App::AO_Blinky1->activate();  }
#else
// use reserved IRQs for SST Tasks
void Reserved42_IRQHandler(void);  // prototype
void Reserved64_IRQHandler(void);  // prototype
void Reserved65_IRQHandler(void);  // prototype
void Reserved66_IRQHandler(void);  // prototype
void Reserved67_IRQHandler(void);  // prototype
void Reserved79_IRQHandler(void);  // prototype
void Reserved124_IRQHandler(void); // prototype
void Reserved123_IRQHandler(void); // prototype
void Reserved126_IRQHandler(void); // prototype
void Reserved143_IRQHandler(void); // prototype
void Reserved145_IRQHandler(void); // prototype
void Reserved147_IRQHandler(void); // prototype
void Reserved148_IRQHandler(void); // prototype

void Reserved42_IRQHandler(void) { App::AO_Blinky3->activate();  }
void Reserved64_IRQHandler(void) { App::AO_Button2b->activate(); }
void Reserved65_IRQHandler(void) { App::AO_Button2a->activate(); }
void Reserved66_IRQHandler(void) { App::AO_Blinky1->activate();  }
#endif

} // extern "C"

namespace BSP {

// BSP functions =============================================================
void init(void) {
    SCB_EnableICache(); // Enable I-Cache
    SCB_EnableDCache(); // Enable D-Cache

    // assign IRQs to tasks. NOTE: critical for SST...
#ifdef REGULAR_IRQS
    // repurpose regular IRQs for SST Tasks
    App::AO_Blinky3->setIRQ(OTG_FS_EP1_OUT_IRQn);
    App::AO_Button2b->setIRQ(OTG_FS_EP1_IN_IRQn);
    App::AO_Button2a->setIRQ(OTG_FS_WKUP_IRQn);
    App::AO_Blinky1->setIRQ(OTG_FS_IRQn);
#else
    // use reserved IRQs for SST Tasks
    App::AO_Blinky3->setIRQ(42U);
    App::AO_Button2b->setIRQ(64U);
    App::AO_Button2a->setIRQ(65U);
    App::AO_Blinky1->setIRQ(66U);
#endif

    // enable GPIOB port clock for LEds and test pins
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;

    /* set all used GPIOB pins as push-pull output, no pull-up, pull-down */
    GPIOB->MODER &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));
    GPIOB->MODER |=
         ((1U << 2U*TST1_PIN) | (1U << 2U*TST2_PIN) | (1U << 2U*TST3_PIN) |
          (1U << 2U*TST4_PIN) | (1U << 2U*TST5_PIN) | (1U << 2U*TST6_PIN));
    GPIOB->OTYPER &=
        ~((1U <<    TST1_PIN) | (1U <<    TST2_PIN) | (1U <<    TST3_PIN) |
          (1U <<    TST4_PIN) | (1U <<    TST5_PIN) | (1U <<    TST6_PIN));
    GPIOB->PUPDR &=
        ~((3U << 2U*TST1_PIN) | (3U << 2U*TST2_PIN) | (3U << 2U*TST3_PIN) |
          (3U << 2U*TST4_PIN) | (3U << 2U*TST5_PIN) | (3U << 2U*TST6_PIN));

    // enable GPIOC clock port for the Button B1
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;

    // configure Button B1 pin on GPIOC as input, no pull-up, pull-down
    GPIOC->MODER   &= ~(3U << 2U*B1_PIN);
    GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPD0 << 2U*B1_PIN);
    GPIOC->PUPDR   |=  (2U << 2U*B1_PIN);
}

//............................................................................
static void exerciseFPU(double x) {
    // exercise the FPU by calculating trigonometric identity
    double tmp = pow(sin(x), 2.0) + pow(cos(x), 2.0);
    Q_ENSURE(((1.0 - 1e-4) < tmp) && (tmp < (1.0 + 1e-4)));
}

//............................................................................
void d1on(void) {  // LED1-Green */
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST1_PIN);
    SST_PORT_CRIT_EXIT();
    /* don't use the FPU in the ISR */
}
void d1off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST1_PIN + 16U));
    SST_PORT_CRIT_EXIT();
    /* don't use the FPU in the ISR */
}
//............................................................................
void d2on(void) {  /* LED3-Red */
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST2_PIN);
    SST_PORT_CRIT_EXIT();

    exerciseFPU(-1.2345);
}
void d2off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST2_PIN + 16U));
    SST_PORT_CRIT_EXIT();

    exerciseFPU(1.2345);
}
//............................................................................
void d3on(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST3_PIN);
    SST_PORT_CRIT_EXIT();

    exerciseFPU(-12.345);
}
void d3off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST3_PIN + 16U));
    SST_PORT_CRIT_EXIT();

    exerciseFPU(12.345);
}
//............................................................................
void d4on(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST4_PIN);
    SST_PORT_CRIT_EXIT();

    exerciseFPU(3.456);
}
void d4off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST4_PIN + 16U));
    SST_PORT_CRIT_EXIT();

    exerciseFPU(-3.456);
}
//............................................................................
void d5on(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST5_PIN);
    SST_PORT_CRIT_EXIT();

    exerciseFPU(4.567);
}
void d5off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST5_PIN + 16U));
    SST_PORT_CRIT_EXIT();

    exerciseFPU(-4.567);
}
//............................................................................
void d6on(void) {  /* LED2-Blue */
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << TST6_PIN);
    SST_PORT_CRIT_EXIT();

    exerciseFPU(1.2345);
}
void d6off(void) {
    SST_PORT_CRIT_STAT
    SST_PORT_CRIT_ENTRY();
    GPIOB->BSRR |= (1U << (TST6_PIN + 16U));
    SST_PORT_CRIT_EXIT();

    exerciseFPU(-1.2345);
}

//............................................................................
SST::Evt const *getWorkEvtBlinky1(uint8_t num) {
    // immutable work events for Blinky1
    static App::BlinkyWorkEvt const workBliny1[] = {
        { { App::BLINKY_WORK_SIG }, 40U, 5U },
        { { App::BLINKY_WORK_SIG }, 30U, 7U }
    };
    Q_REQUIRE(num < Q_DIM(workBliny1)); // num must be in range
    return &workBliny1[num].super;
}
//............................................................................
SST::Evt const *getWorkEvtBlinky3(uint8_t num) {
    /* immutable work events for Blinky3 */
    static App::BlinkyWorkEvt const workBlinky3[] = {
        { { App::BLINKY_WORK_SIG }, 20U, 5U },
        { { App::BLINKY_WORK_SIG }, 10U, 3U   }
    };
    Q_REQUIRE(num < Q_DIM(workBlinky3)); // num must be in range
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

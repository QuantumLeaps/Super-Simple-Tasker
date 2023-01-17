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
#include "sst.h"
#include "bsp.h"
#include "blinky_button.h"

#include "stm32l0xx.h"  /* CMSIS-compliant header file for the MCU used */
/* add other drivers if necessary... */

/* Local-scope defines -----------------------------------------------------*/

/* Test pins on PA */
#define PIN_PA0  (1U << 0)
#define PIN_PA1  (1U << 1)
#define PIN_PA4  (1U << 4)
#define PIN_PA5  (1U << 5) /* LED LD2-Green on PA.5) */
#define PIN_PA6  (1U << 6)
#define PIN_PA7  (1U << 7)

/* Button B1 on PC.13 */
#define BTN_B1   (1U << 13)

/* ISRs used in the application ============================================*/
void SysTick_Handler(void) {   /* system clock tick ISR */
    BSP_d1on();

    /* immutable timeout event */
    static SST_Evt const tickEvt = { TICK_SIG };
    SST_Task_post(AO_Blinky1, &tickEvt); /* every tick is fast for Blinky1 */
    SST_Task_post(AO_Blinky3, &tickEvt);

    /* get state of the user button */
    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current = ~GPIOC->IDR; /* read GPIO PortC */
    uint32_t tmp = buttons.depressed; /* save the debounced depressed */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_B1) != 0U) {   /* debounced B1 state changed? */
        if ((buttons.depressed & BTN_B1) != 0U) { /* is B1 depressed? */
            /* immutable button-press event */
            static ButtonWorkEvt const pressEvt = {
                .super.sig = BUTTON_PRESSED_SIG,
                .toggles = 60U
            };
            SST_Task_post(AO_Button2a, &pressEvt.super);
        }
        else { /* B1 is released */
            /* immutable button-release event */
            static ButtonWorkEvt const releaseEvt = {
                .super.sig = BUTTON_RELEASED_SIG,
                .toggles = 80U
            };
            SST_Task_post(AO_Button2a, &releaseEvt.super);
        }
    }

    BSP_d1off();
}

/* SST task activations ====================================================*/
/* preprocessor switch to choose between regular and reserved IRQs */
//#define REGULAR_IRQS

#ifdef REGULAR_IRQS
/* repurpose regular IRQs for SST Tasks */
void PVD_IRQHandler(void)  { SST_Task_activate(AO_Blinky3);  }
void RTC_IRQHandler(void)  { SST_Task_activate(AO_Button2b); }
void TSC_IRQHandler(void)  { SST_Task_activate(AO_Button2a); }
void I2C2_IRQHandler(void) { SST_Task_activate(AO_Blinky1);  }
#else
/* use reserved IRQs for SST Tasks */
void Reserved14_IRQHandler(void); /* pototype */
void Reserved16_IRQHandler(void); /* pototype */
void Reserved18_IRQHandler(void); /* pototype */
void Reserved19_IRQHandler(void); /* pototype */
void Reserved21_IRQHandler(void); /* pototype */

void Reserved14_IRQHandler(void) { SST_Task_activate(AO_Blinky3);  }
void Reserved16_IRQHandler(void) { SST_Task_activate(AO_Button2b); }
void Reserved18_IRQHandler(void) { SST_Task_activate(AO_Button2a); }
void Reserved19_IRQHandler(void) { SST_Task_activate(AO_Blinky1);  }
#endif

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* assign IRQs to tasks. NOTE: critical for SST... */
#ifdef REGULAR_IRQS
    /* repurpose regular IRQs for SST Tasks */
    SST_Task_setIRQ(AO_Blinky3,  PVD_IRQn);
    SST_Task_setIRQ(AO_Button2b, RTC_IRQn);
    SST_Task_setIRQ(AO_Button2a, TSC_IRQn);
    SST_Task_setIRQ(AO_Blinky1,  I2C2_IRQn);
#else
    /* use reserved IRQs for SST Tasks */
    SST_Task_setIRQ(AO_Blinky3,  14U);
    SST_Task_setIRQ(AO_Button2b, 16U);
    SST_Task_setIRQ(AO_Button2a, 18U);
    SST_Task_setIRQ(AO_Blinky1,  19U);
#endif

    /* enable GPIO port PA clock */
    RCC->IOPENR |= (1U << 0);

    /* set all used PA pins as push-pull output, no pull-up, pull-down */
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

    /* enable GPIOC clock port for the Button B1 */
    RCC->IOPENR |=  (1U << 2);

    /* configure Button (PC.13) pin as input, no pull-up, pull-down */
    GPIOC->MODER   &= ~(3U << 2*13);
    GPIOC->OSPEEDR &= ~(3U << 2*13);
    GPIOC->OSPEEDR |=  (1U << 2*13);
    GPIOC->PUPDR   &= ~(3U << 2*13);
}
/*..........................................................................*/
void BSP_d1on(void)  { GPIOA->BSRR |= (PIN_PA7);       }
void BSP_d1off(void) { GPIOA->BSRR |= (PIN_PA7 << 16); }
/*..........................................................................*/
void BSP_d2on(void)  { GPIOA->BSRR |= (PIN_PA6);       }
void BSP_d2off(void) { GPIOA->BSRR |= (PIN_PA6 << 16); }
/*..........................................................................*/
void BSP_d3on(void)  { GPIOA->BSRR |= (PIN_PA4);       }
void BSP_d3off(void) { GPIOA->BSRR |= (PIN_PA4 << 16); }
/*..........................................................................*/
void BSP_d4on(void)  { GPIOA->BSRR |= (PIN_PA1);       }
void BSP_d4off(void) { GPIOA->BSRR |= (PIN_PA1 << 16); }
/*..........................................................................*/
void BSP_d5on(void)  { GPIOA->BSRR |= (PIN_PA0);       }
void BSP_d5off(void) { GPIOA->BSRR |= (PIN_PA0 << 16); }
/*..........................................................................*/
void BSP_d6on(void)  { GPIOA->BSRR |= (PIN_PA5);       }
void BSP_d6off(void) { GPIOA->BSRR |= (PIN_PA5 << 16); }

/*..........................................................................*/
SST_Evt const *getInitEvtBlinky3(void) {
    static BlinkyWorkEvt const initBlinky3 = {
        .super.sig = BLINKY_WORK_SIG,
        .toggles = 10U,
        .ticks = 3U,
    };
    return &initBlinky3.super;
}
/*..........................................................................*/
SST_Evt const *getInitEvtBlinky1(void) {
    static BlinkyWorkEvt const initBlinky1 = {
        .super.sig = BLINKY_WORK_SIG,
        .toggles = 30U,
        .ticks = 7U,
    };
    return &initBlinky1.super;
}

/* SST callbacks ===========================================================*/
void SST_onStart(void) {
    SystemCoreClockUpdate();

    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config((SystemCoreClock / BSP_TICKS_PER_SEC) + 1U);

    /* set priorities of ISRs used in the system */
    NVIC_SetPriority(SysTick_IRQn, 0U);
    /* ... */
}
/*..........................................................................*/
void SST_onIdle(void) {
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    /* !!!CAUTION!!!
    * The WFI instruction stops the CPU clock, which unfortunately disables
    * the JTAG port, so the ST-Link debugger can no longer connect to the
    * board. For that reason, the call to __WFI() has to be used with CAUTION.
    *
    * NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    * reset the board, then connect with ST-Link Utilities and erase the part.
    * The trick with BOOT(0) is it gets the part to run the System Loader
    * instead of your broken code. When done disconnect BOOT0, and start over.
    */
    __WFI(); /* Wait-For-Interrupt */
#else
    BSP_d6on();  /* turn LED2 on */
    BSP_d6off(); /* turn LED2 off */
#endif
}

/* Assertion handler ======================================================*/
void Q_onAssert(char const * const module, int const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;

    /* set PRIMASK to disable interrupts and stop SST right here */
    __asm volatile ("cpsid i");

#ifndef NDEBUG
    for (;;) { /* keep blinking LED2 */
        BSP_d6on();  /* turn LED2 on */
        uint32_t volatile ctr;
        for (ctr = 10000U; ctr != 0U; --ctr) {
        }
        BSP_d6off(); /* turn LED2 off */
        for (ctr = 10000U; ctr != 0U; --ctr) {
        }
    }
#endif
    NVIC_SystemReset();
}
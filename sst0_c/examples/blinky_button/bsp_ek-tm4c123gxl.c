/*============================================================================
* Super-Simple Tasker (SST0/C) Example for TivaC TM4C123GXL
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

#include "TM4C123GH6PM.h" /* the device specific header (TI) */
#include <math.h>         /* to exercise the FPU */
/* add other drivers if necessary... */

DBC_MODULE_NAME("bsp_ek-tm4c123gxl") /* for DBC assertions in this module */

/* Local-scope defines -----------------------------------------------------*/

/* test pins on GPIOF */
#define TST1_PIN  (1U << 1U) /* LED Red */
#define TST2_PIN  (1U << 2U) /* LED Blue */

/* test pins on GPIOD */
#define TST3_PIN  (1U << 0U)
#define TST4_PIN  (1U << 1U)
#define TST5_PIN  (1U << 2U)

/* test pins on GPIOF */
#define TST6_PIN  (1U << 3U) /* LED Green */

/* Button on the board on GPIOF */
#define BTN_SW1      (1U << 4)

/* ISRs used in the application ============================================*/
void SysTick_Handler(void);  /* prototype */

void SysTick_Handler(void) { /* system clock tick ISR */
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
    uint32_t current = ~GPIOF_AHB->DATA_Bits[BTN_SW1];
    uint32_t tmp = buttons.depressed; /* save the debounced depressed */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_SW1) != 0U) {  /* debounced SW1 state changed? */
        if ((buttons.depressed & BTN_SW1) != 0U) { /* is SW1 depressed? */
            /* immutable button-press event */
            static ButtonWorkEvt const pressEvt = {
                .super.sig = BUTTON_PRESSED_SIG,
                .toggles = 60U
            };
            /* immutable forward-press event */
            static ButtonWorkEvt const fPressEvt = {
                .super.sig = FORWARD_PRESSED_SIG,
                .toggles = 60U
            };
            SST_Task_post(AO_Button2a, &fPressEvt.super);
            SST_Task_post(AO_Button2a, &pressEvt.super);
        }
        else { /* B1 is released */
            /* immutable button-release event */
            static ButtonWorkEvt const releaseEvt = {
                .super.sig = BUTTON_RELEASED_SIG,
                .toggles = 80U
            };
            /* immutable forward-release event */
            static ButtonWorkEvt const fReleaseEvt = {
                .super.sig = FORWARD_RELEASED_SIG,
                .toggles = 80U
            };
            SST_Task_post(AO_Button2a, &fReleaseEvt.super);
            SST_Task_post(AO_Button2a, &releaseEvt.super);
        }
    }

    BSP_d1off();
}

/* Assertion handler ======================================================*/
void DBC_fault_handler(char const * const module, int const label) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)label;

    /* set PRIMASK to disable interrupts and stop SST right here */
    __asm volatile ("cpsid i");

#ifndef NDEBUG
    for (;;) { /* keep blinking LED2 */
        BSP_d6on();  /* turn LED2 on */
        uint32_t volatile ctr;
        for (ctr = 1000000U; ctr > 0U; --ctr) {
        }
        BSP_d6off(); /* turn LED2 off */
        for (ctr = 1000000U; ctr > 0U; --ctr) {
        }
    }
#endif
    NVIC_SystemReset();
}
/* BSP functions ===========================================================*/
void BSP_init(void) {

    SYSCTL->RCGCGPIO  |= (1U << 5U); /* enable Run mode for GPIOF */
    SYSCTL->RCGCGPIO  |= (1U << 3U); /* enable Run mode for GPIOD */
    __ISB();
    __DSB();

    SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */
    SYSCTL->GPIOHBCTL |= (1U << 3); /* enable AHB for GPIOD */
    __ISB();
    __DSB();

    /* configure test pins on GPIOF (digital output) */
    GPIOF_AHB->DIR |= (TST1_PIN | TST2_PIN | TST6_PIN);
    GPIOF_AHB->DEN |= (TST1_PIN | TST2_PIN | TST6_PIN);

    /* configure button on GPIOF (digital input) */
    GPIOF_AHB->DIR &= ~(BTN_SW1); /* input */
    GPIOF_AHB->DEN |= (BTN_SW1); /* digital enable */
    GPIOF_AHB->PUR |= (BTN_SW1); /* pull-up resistor enable */

    /* configure test pins on GPIOD (digital output) */
    GPIOD_AHB->DIR |= (TST3_PIN | TST4_PIN | TST5_PIN);
    GPIOD_AHB->DEN |= (TST3_PIN | TST4_PIN | TST5_PIN);
}

/*..........................................................................*/
#if defined __ARMCC_VERSION
#elif defined __GNUC__
uint32_t __errno; /* GNU-ARM needs this to link sqrtf() */
#endif

static void exerciseFPU(float x) {
    /* exercise the single-precision FPU by calculating the identity:
    *  sqrt(x) == x / sqrt(x) for x > 0
    */
    float tmp1 = sqrtf(x); /* single-precision sqrt() */
    float tmp2 = x / tmp1;
    DBC_ENSURE(200, (tmp1 - 1e-4f <= tmp2) && (tmp2 <= tmp1 + 1e-4f));
}

/*..........................................................................*/
void BSP_d1on(void) { /* LED-Red */
    GPIOF_AHB->DATA_Bits[TST1_PIN] = 0xFFU;
    /* don't use the FPU in the ISR */
}
void BSP_d1off(void) {
    GPIOF_AHB->DATA_Bits[TST1_PIN] = 0x00U;
}
/*..........................................................................*/
void BSP_d2on(void) { /* LED-Blue */
    GPIOF_AHB->DATA_Bits[TST2_PIN] = 0xFFU;
    exerciseFPU(1.2345f);
}
void BSP_d2off(void) {
    GPIOF_AHB->DATA_Bits[TST2_PIN] = 0x00U;
}
/*..........................................................................*/
void BSP_d3on(void) {
    GPIOD_AHB->DATA_Bits[TST3_PIN] = 0xFFU;
    exerciseFPU(0.345f);
}
void BSP_d3off(void) {
    GPIOD_AHB->DATA_Bits[TST3_PIN] = 0x00U;
}
/*..........................................................................*/
void BSP_d4on(void) {
    GPIOD_AHB->DATA_Bits[TST4_PIN] = 0xFFU;
    exerciseFPU(0.456f);
}
void BSP_d4off(void) {
    GPIOD_AHB->DATA_Bits[TST4_PIN] = 0x00U;
}
/*..........................................................................*/
void BSP_d5on(void) {
    GPIOD_AHB->DATA_Bits[TST5_PIN] = 0xFFU;
    exerciseFPU(1.567f);
}
void BSP_d5off(void) {
    GPIOD_AHB->DATA_Bits[TST5_PIN] = 0x00U;
}
/*..........................................................................*/
void BSP_d6on(void) {  /* LED2-Green */
    GPIOF_AHB->DATA_Bits[TST6_PIN] = 0xFFU;
    exerciseFPU(1.2345f);
}
void BSP_d6off(void) {
    GPIOF_AHB->DATA_Bits[TST6_PIN] = 0x00U;
}

/*..........................................................................*/
SST_Evt const *BSP_getWorkEvtBlinky1(uint8_t num) {
    /* immutable work events for Blinky1 */
    static BlinkyWorkEvt const workBliny1[] = {
        {
            .super.sig = BLINKY_WORK_SIG,
            .toggles = 40U,
            .ticks = 5U,
        },
        {
            .super.sig = BLINKY_WORK_SIG,
            .toggles = 30U,
            .ticks = 7U,
        }
    };
    DBC_REQUIRE(500,
        num < ARRAY_NELEM(workBliny1)); /* num must be in range */
    return &workBliny1[num].super;
}
/*..........................................................................*/
SST_Evt const *BSP_getWorkEvtBlinky3(uint8_t num) {
    /* immutable work events for Blinky3 */
    static BlinkyWorkEvt const workBlinky3[] = {
        {
            .super.sig = BLINKY_WORK_SIG,
            .toggles = 20U,
            .ticks = 5U,
        },
        {
            .super.sig = BLINKY_WORK_SIG,
            .toggles = 10U,
            .ticks = 3U,
        }
    };
    DBC_REQUIRE(600,
        num < ARRAY_NELEM(workBlinky3)); /* num must be in range */
    return &workBlinky3[num].super;
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
void SST_onIdleCond(void) { /* NOTE: called with interrupts DISABLED */
    BSP_d6on();  /* turn LED-Green on */
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    BSP_d6off(); /* turn LED-Green off */
    __WFI(); /* Wait-For-Interrupt */
    BSP_d6on();  /* turn LED-Green on */
#endif
    BSP_d6off(); /* turn LED-Green off */
    SST_PORT_INT_ENABLE(); /* NOTE: enable interrupts for SS0 */
}


/*============================================================================
* Super-Simple Tasker (SST0/C) Example for STM32 NUCLEO-H743ZI
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, <state-machine.com>.
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

#include "stm32h743xx.h"  /* CMSIS-compliant header file for the MCU used */
#include <math.h>         /* to exercise the FPU */
/* add other drivers if necessary... */

DBC_MODULE_NAME("bsp_nucleo-h743zi") /* for DBC assertions in this module */

/* Local-scope defines -----------------------------------------------------*/

/* test pins on GPIO PB */
#define TST1_PIN  0U  /* PB.0  LED1-Green */
#define TST2_PIN  14U /* PB.14 LED3-Red   */
#define TST3_PIN  4U
#define TST4_PIN  5U
#define TST5_PIN  6U
#define TST6_PIN  7U  /* PB.7  LED2-Blue  */

/* buttons on GPIO PC */
#define B1_PIN    13U

/* ISRs used in the application ============================================*/
void SysTick_Handler(void);  /* prototype */

void SysTick_Handler(void) { /* system clock tick ISR */
    BSP_d1on();

    /* immutable timeout event */
    static SST_Evt const tickEvt = { TICK_SIG };
    SST_Task_post(AO_Blinky1, &tickEvt); /* every tick is fast for Blinky1 */
    SST_Task_post(AO_Blinky3, &tickEvt);

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current = GPIOC->IDR; /* read GPIO PortC */
    uint32_t tmp = buttons.depressed; /* save the debounced depressed */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & (1U << B1_PIN)) != 0U) { /* debounced B1 state changed? */
        if ((buttons.depressed & (1U << B1_PIN)) != 0U) { /* depressed? */
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
DBC_NORETURN
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
/*..........................................................................*/
void assert_failed(char const * const module, int const label);/* prototype */
void assert_failed(char const * const module, int const label) {
    DBC_fault_handler(module, label);
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* Configure the MPU to prevent NULL-pointer dereferencing
    * see: www.state-machine.com/null-pointer-protection-with-arm-cortex-m-mpu
    */
    MPU->RBAR = 0x0U                          /* base address (NULL) */
                | MPU_RBAR_VALID_Msk          /* valid region */
                | (MPU_RBAR_REGION_Msk & 7U); /* region #7 */
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     /* 2^(7+1) region */
                | (0x0U << MPU_RASR_AP_Pos)   /* no-access region */
                | MPU_RASR_ENABLE_Msk;        /* region enable */

    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       /* enable background region */
                | MPU_CTRL_ENABLE_Msk;        /* enable the MPU */
    __ISB();
    __DSB();

    SCB_EnableICache(); /* Enable I-Cache */
    SCB_EnableDCache(); /* Enable D-Cache */

    /* enable GPIOB port clock for LEds and test pins */
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

    /* enable GPIOC clock port for the Button B1 */
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;

    /* configure Button B1 pin on GPIOC as input, no pull-up, pull-down */
    GPIOC->MODER   &= ~(3U << 2U*B1_PIN);
    GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPD0 << 2U*B1_PIN);
    GPIOC->PUPDR   |=  (2U << 2U*B1_PIN);
}

/*..........................................................................*/
static void exerciseFPU(double x) {
    /* exercise the double-precision FPU by calculating the identity:
    *  sin(x)^2 + cos(x)^2 == 1.0 for any x
    */
    double tmp = pow(sin(x), 2.0) + pow(cos(x), 2.0);
    DBC_ENSURE(200, (1.0 - 1e-4 < tmp) && (tmp < 1.0 + 1e-4));
}

/*..........................................................................*/
void BSP_d1on(void) {  /* LED1-Green */
    GPIOB->BSRR = (1U << TST1_PIN);
    /* don't use the FPU in the ISR */
}
void BSP_d1off(void) {
    GPIOB->BSRR = (1U << (TST1_PIN + 16U));
}
/*..........................................................................*/
void BSP_d2on(void) {  /* LED3-Red */
    GPIOB->BSRR = (1U << TST2_PIN);
    exerciseFPU(-1.2345);
}
void BSP_d2off(void) {
    GPIOB->BSRR = (1U << (TST2_PIN + 16U));
}
/*..........................................................................*/
void BSP_d3on(void) {
    GPIOB->BSRR = (1U << TST3_PIN);
    exerciseFPU(-12.345);
}
void BSP_d3off(void) {
    GPIOB->BSRR = (1U << (TST3_PIN + 16U));
}
/*..........................................................................*/
void BSP_d4on(void) {
    GPIOB->BSRR = (1U << TST4_PIN);
    exerciseFPU(3.456);
}
void BSP_d4off(void) {
    GPIOB->BSRR = (1U << (TST4_PIN + 16U));
}
/*..........................................................................*/
void BSP_d5on(void) {
    GPIOB->BSRR = (1U << TST5_PIN);
    exerciseFPU(4.567);
}
void BSP_d5off(void) {
    GPIOB->BSRR = (1U << (TST5_PIN + 16U));
}
/*..........................................................................*/
void BSP_d6on(void) {  /* LED2-Blue */
    GPIOB->BSRR = (1U << TST6_PIN);
    exerciseFPU(1.2345);
}
void BSP_d6off(void) {
    GPIOB->BSRR = (1U << (TST6_PIN + 16U));
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
    DBC_REQUIRE(500, num < ARRAY_NELEM(workBliny1)); /* must be in range */
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
    DBC_REQUIRE(600, num < ARRAY_NELEM(workBlinky3)); /* must be in range */
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
    BSP_d6on();  /* turn LED2 on */
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    BSP_d6off(); /* turn LED2 off */
    __WFI(); /* Wait-For-Interrupt */
    BSP_d6on();  /* turn LED2 on */
#endif
    BSP_d6off(); /* turn LED2 off */
    SST_PORT_INT_ENABLE(); /* NOTE: enable interrupts for SS0 */
}


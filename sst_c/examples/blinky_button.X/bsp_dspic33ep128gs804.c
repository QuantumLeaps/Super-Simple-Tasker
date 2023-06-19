/*============================================================================
* Super-Simple Tasker (SST/C) Example for TivaC TM4C123GXL
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
#include "system_dspic33ep128gs804.h"       /* dspic33ep drivers and register map*/
#include <math.h>          /* to exercise the FPU */
/* add other drivers if necessary... */

DBC_MODULE_NAME("bsp_dspic") /* for DBC assertions in this module */

/* Local-scope defines -----------------------------------------------------*/


/* test pins on GPIOA */
#define TST1_PORT GPIOA
#define TST1_PIN  PIN_4 /* LED Red */
#define TST2_PORT GPIOA
#define TST2_PIN  PIN_3 /* LED Blue */

/* test pins on GPIOD */
#define TST3_PORT GPIOB
#define TST3_PIN  PIN_11
#define TST4_PORT GPIOB
#define TST4_PIN  PIN_12
#define TST5_PORT GPIOB
#define TST5_PIN  PIN_13

/* test pins on GPIOA */
#define TST6_PORT GPIOB
#define TST6_PIN  PIN_14 /* LED Green */

/* Button on the board on GPIOF */
#define BTN_SW1_PORT GPIOC
#define BTN_SW1      PIN_3

/* ISRs used in the application ============================================*/
void SysTick_Handler(void);  /* prototype */

/* Timer1 ISR */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0; //Clear Timer1 interrupt flag
    SysTick_Handler();
}

void SysTick_Handler(void) { /* system clock tick ISR */
    BSP_d1on();

    /* immutable timeout event */
    static SST_Evt const tickEvt = { TIMEOUT_SIG };
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
    uint32_t current = ~((BTN_SW1_PORT->PORT) & (BTN_SW1));
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
DBC_NORETURN
void DBC_fault_handler(char const * const module, int const label) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)label;

    /* set PRIMASK to disable interrupts and stop SST right here */
    SST_PORT_CRIT_ENTRY();

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
    dspic_SystemReset();
}
/*..........................................................................*/
void assert_failed(char const * const module, int const label);/* prototype */
void assert_failed(char const * const module, int const label) {
    DBC_fault_handler(module, label);
}

/* SST task activations ====================================================*/

/* repurpose regular IRQs for SST Tasks */
void __attribute__((__interrupt__, no_auto_psv)) _PTG0Interrupt(void) {
    IFS9bits.PTG0IF = 0;
    SST_Task_activate(AO_Blinky3);
}

void __attribute__((__interrupt__, no_auto_psv)) _PTG1Interrupt(void) {
    IFS9bits.PTG1IF = 0;
    SST_Task_activate(AO_Button2b);
}

void __attribute__((__interrupt__, no_auto_psv)) _PTG2Interrupt(void) {
    IFS9bits.PTG2IF = 0;
    SST_Task_activate(AO_Button2a);
}

void __attribute__((__interrupt__, no_auto_psv)) _PTG3Interrupt(void) {
    IFS9bits.PTG3IF = 0;
    SST_Task_activate(AO_Blinky1);
}

/* BSP functions ===========================================================*/
void BSP_init(void) {

    /* assign IRQs to tasks. NOTE: critical for SST... */
    SST_Task_setIRQ(AO_Blinky3,  _PTG0Interrupt_n);
    SST_Task_setIRQ(AO_Button2b, _PTG1Interrupt_n);
    SST_Task_setIRQ(AO_Button2a, _PTG2Interrupt_n);
    SST_Task_setIRQ(AO_Blinky1,  _PTG3Interrupt_n);


    /* configure test pins on GPIOA (digital output) */
    //gpio_set_direction(GPIOA, TST1_PIN | TST2_PIN | TST6_PIN, GPIO_OUTPUT);

    /* configure button on GPIOF (digital input) */
    //gpio_set_direction(GPIOA, BTN_SW1, GPIO_INPUT_PU);
    /* configure test pins on GPIOD (digital output) */
    //gpio_set_direction(GPIOB, TST3_PIN | TST4_PIN | TST5_PIN, GPIO_OUTPUT);
    
    gpio_set_direction(TST1_PORT, TST1_PIN, GPIO_OUTPUT);
    gpio_set_direction(TST2_PORT, TST2_PIN, GPIO_OUTPUT);
    gpio_set_direction(TST3_PORT, TST3_PIN, GPIO_OUTPUT);
    gpio_set_direction(TST4_PORT, TST4_PIN, GPIO_OUTPUT);
    gpio_set_direction(TST5_PORT, TST5_PIN, GPIO_OUTPUT);
    gpio_set_direction(TST6_PORT, TST6_PIN, GPIO_OUTPUT);
    gpio_set_direction(BTN_SW1_PORT, BTN_SW1, GPIO_INPUT_PU);
}

/*..........................................................................*/
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
    gpio_set_level(TST1_PORT ,TST1_PIN, 1);
    /* don't use the FPU in the ISR */
}
void BSP_d1off(void) {
    gpio_set_level(TST1_PORT ,TST1_PIN, 0);
}
/*..........................................................................*/
void BSP_d2on(void) { /* LED-Blue */
    gpio_set_level(TST2_PORT ,TST2_PIN, 1);
    exerciseFPU(1.2345f);
}
void BSP_d2off(void) {
    gpio_set_level(TST2_PORT ,TST2_PIN, 0);
}
/*..........................................................................*/
void BSP_d3on(void) {
    gpio_set_level(TST3_PORT ,TST3_PIN, 1);
    exerciseFPU(0.345f);
}
void BSP_d3off(void) {
    gpio_set_level(TST3_PORT ,TST3_PIN, 0);
}
/*..........................................................................*/
void BSP_d4on(void) {
    gpio_set_level(TST4_PORT ,TST4_PIN, 1);
    exerciseFPU(0.456f);
}
void BSP_d4off(void) {
    gpio_set_level(TST4_PORT ,TST4_PIN, 0);
}
/*..........................................................................*/
void BSP_d5on(void) {
    gpio_set_level(TST5_PORT ,TST5_PIN, 1);
    exerciseFPU(1.567f);
}
void BSP_d5off(void) {
    gpio_set_level(TST5_PORT ,TST5_PIN, 0);
}
/*..........................................................................*/
void BSP_d6on(void) {  /* LED2-Green */
    gpio_set_level(TST6_PORT ,TST6_PIN, 1);
    exerciseFPU(1.2345f);
}
void BSP_d6off(void) {
    gpio_set_level(TST6_PORT ,TST6_PIN, 0);
}

/*..........................................................................*/
SST_Evt const *BSP_getWorkEvtBlinky1(uint8_t num) {
    /* immutable work events for Blinky1 */
    static BlinkyWorkEvt const workBlinky1[] = {
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
    DBC_REQUIRE(500, num < ARRAY_NELEM(workBlinky1)); /* must be in range */
    return &workBlinky1[num].super;
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

    /* set up the SysTick timer "timer1" to fire at BSP_TICKS_PER_SEC rate */
    volatile uint16_t temp = BSP_TICKS_PER_SEC;
    SysTick_Config(temp);

    /* set priorities of ISRs used in the system */
    interrupt_SetPriority(_T1Interrupt_n, 7U);  // tick timer highest priority
    /* ... */
}
/*..........................................................................*/
void SST_onIdle(void) {
    BSP_d6on();  /* turn LED-Green on */
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular MCU.
    */
    BSP_d6off(); /* turn LED-Green off */
    Idle(); /* Wait-For-Interrupt */
    BSP_d6on();  /* turn LED-Green on */
#endif
    BSP_d6off(); /* turn LED-Green off */
}


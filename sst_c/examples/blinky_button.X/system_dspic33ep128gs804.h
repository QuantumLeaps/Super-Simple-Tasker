/* 
 * File:   drivers.h
 * Author: ASHRAF 
 *
 * Created on June 12, 2023, 3:17 PM
 */

#ifndef DRIVERS_H
#define	DRIVERS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "xc.h"  /* the device specific header */

    
    /********* GPIO REGISTERS ********/

typedef volatile struct { // GPIO Structure
  uint16_t TRIS;
  uint16_t PORT;
  uint16_t LAT;
  uint16_t ODC;

  uint16_t CNEN;
  uint16_t CNPU;
  uint16_t CNPD;
  uint16_t ANSEL;
} *const gpio_t;

#define GPIOA_BASE 0xE00
#define GPIOB_BASE 0xE10
#define GPIOC_BASE 0xE20
#define GPIO_CHANGE_BASE 0xE08

typedef enum {
  _INT0Interrupt_n = 0,
  _IC1Interrupt_n = 1,
  _OC1Interrupt_n = 2,
  _T1Interrupt_n = 3,
  _DMA0Interrupt_n = 4,
  _IC2Interrupt_n = 5,
  _OC2Interrupt_n = 6,
  _T2Interrupt_n = 7,
  _T3Interrupt_n = 8,
  _SPI1TXInterrupt_n = 9,
  _SPI1RXInterrupt_n = 10,
  _U1RXInterrupt_n = 11,
  _U1TXInterrupt_n = 12,
  _AD1Interrupt_n = 13,
  _DMA1Interrupt_n = 14,
  _NVMInterrupt_n = 15,
  _SI2C1Interrupt_n = 16,
  _MI2C1Interrupt_n = 17,
  _CMP1Interrupt_n = 18,
  _CNInterrupt_n = 19,
  _INT1Interrupt_n = 20,
  _DMA2Interrupt_n = 24,
  _OC3Interrupt_n = 25,
  _OC4Interrupt_n = 26,
  _T4Interrupt_n = 27,
  _T5Interrupt_n = 28,
  _INT2Interrupt_n = 29,
  _U2RXInterrupt_n = 30,
  _U2TXInterrupt_n = 31,
  _SPI2TXInterrupt_n = 32,
  _SPI2RXInterrupt_n = 33,
  _C1RxInterrupt_n = 34,
  _C1Interrupt_n = 35,
  _DMA3Interrupt_n = 36,
  _IC3Interrupt_n = 37,
  _IC4Interrupt_n = 38,
  _SI2C2Interrupt_n = 49,
  _MI2C2Interrupt_n = 50,
  _INT4Interrupt_n = 54,
  _C2RxInterrupt_n = 55,
  _C2Interrupt_n = 56,
  _PWMSpEventMatchInterrupt_n = 57,
  _U1ErrInterrupt_n = 65,
  _U2ErrInterrupt_n = 66,
  _C1TxInterrupt_n = 70,
  _C2TxInterrupt_n = 71,
  _PWMSecSpEventMatchInterrupt_n = 73,
  _SPI3TXInterrupt_n = 90,
  _SPI3RXInterrupt_n = 91,
  _PWM1Interrupt_n = 94,
  _PWM2Interrupt_n = 95,
  _PWM3Interrupt_n = 96,
  _PWM4Interrupt_n = 97,
  _PWM5Interrupt_n = 98,
  _PWM6Interrupt_n = 99,
  _PWM7Interrupt_n = 100,
  _PWM8Interrupt_n = 101,
  _CMP2Interrupt_n = 103,
  _CMP3Interrupt_n = 104,
  _CMP4Interrupt_n = 105,
  _ADCAN0Interrupt_n = 110,
  _ADCAN1Interrupt_n = 111,
  _ADCAN2Interrupt_n = 112,
  _ADCAN3Interrupt_n = 113,
  _ADCAN4Interrupt_n = 114,
  _ADCAN5Interrupt_n = 115,
  _ADCAN6Interrupt_n = 116,
  _ADCAN7Interrupt_n = 117,
  _SPI1GInterrupt_n = 124,
  _SPI2GInterrupt_n = 125,
  _SPI3GInterrupt_n = 126,
  _CLC1Interrupt_n = 138,
  _CLC2Interrupt_n = 139,
  _CLC3Interrupt_n = 140,
  _CLC4Interrupt_n = 141,
  _ICDInterrupt_n = 142,
  _JTAGInterrupt_n = 143,
  _PTGStepInterrupt_n = 145,
  _PTGWDTInterrupt_n = 146,
  _PTG0Interrupt_n = 147,
  _PTG1Interrupt_n = 148,
  _PTG2Interrupt_n = 149,
  _PTG3Interrupt_n = 150,
  _ADCAN8Interrupt_n = 151,
  _ADCAN9Interrupt_n = 152,
  _ADCAN10Interrupt_n = 153,
  _ADCAN11Interrupt_n = 154,
  _ADCAN12Interrupt_n = 155,
  _ADCAN13Interrupt_n = 156,
  _ADCAN14Interrupt_n = 157,
  _ADCAN15Interrupt_n = 158,
  _ADCAN16Interrupt_n = 159,
  _ADCAN17Interrupt_n = 160,
  _ADCAN18Interrupt_n = 161,
  _ADCAN19Interrupt_n = 162,
  _ADCAN20Interrupt_n = 163,
  _ADCAN21Interrupt_n = 164,
  _I2C1BCInterrupt_n = 173,
  _I2C2BCInterrupt_n = 174,
  _ADCMP0Interrupt_n = 177,
  _ADCMP1Interrupt_n = 178,
  _ADFLTR0Interrupt_n = 179,
  _ADFLTR1Interrupt_n = 180,
  _ADCCore0Interrupt_n = 187,
  _ADCCore1Interrupt_n = 188,
  _ADCCore2Interrupt_n = 189,
  _ADCCore3Interrupt_n = 190
}IRQn_Type;

#define GPIOA      ((gpio_t)GPIOA_BASE)
#define GPIOB      ((gpio_t)GPIOB_BASE)
#define GPIOC      ((gpio_t)GPIOC_BASE)

typedef uint16_t gpio_pin_t;
#define PIN_0       0x0001   //0b0000 0000 0000 0001
#define PIN_1       0x0002   //0b0000 0000 0000 0010
#define PIN_2       0x0004   //0b0000 0000 0000 0100
#define PIN_3       0x0008   //0b0000 0000 0000 1000
#define PIN_4       0x0010   //0b0000 0000 0001 0000
#define PIN_5       0x0020   //0b0000 0000 0010 0000
#define PIN_6       0x0040   //0b0000 0000 0100 0000
#define PIN_7       0x0080   //0b0000 0000 1000 0000
#define PIN_8       0x0100   //0b0000 0001 0000 0000
#define PIN_9       0x0200   //0b0000 0010 0000 0000
#define PIN_10      0x0400   //0b0000 0100 0000 0000
#define PIN_11      0x0800   //0b0000 1000 0000 0000
#define PIN_12      0x1000   //0b0001 0000 0000 0000
#define PIN_13      0x2000   //0b0010 0000 0000 0000
#define PIN_14      0x4000   //0b0100 0000 0000 0000
#define PIN_15      0x8000   //0b1000 0000 0000 0000
#define PIN_ALL     0xffff   //0b1111 1111 1111 1111


#define HIGH 1
#define LOW 0

typedef uint8_t gpio_direction_t;

#define GPIO_INPUT          0     //
#define GPIO_OUTPUT         1     //
#define GPIO_INPUT_PU       2     // Pull Up
#define GPIO_INPUT_PD       3     // Pull Down
#define GPIO_OPEN_DRAIN     4     //
#define GPIO_INPUT_AN       5     //
typedef uint8_t gpio_level_t;

void gpio_set_direction (gpio_t GPIOx, gpio_pin_t pin_no,
                            gpio_direction_t direction);
void gpio_set_level(gpio_t gpio, gpio_pin_t pin_num, int8_t state);
int8_t gpio_get_level(gpio_t gpio, gpio_pin_t pin_num);


void dspic_SystemReset(void);
void SystemCoreClockUpdate(void);
void interrupt_SetPriority(IRQn_Type IRQn, uint8_t priority);
void interrupt_Enable(IRQn_Type IRQn);
void SysTick_Config(uint16_t freq_hz);


#ifdef	__cplusplus
}
#endif

#endif	/* DRIVERS_H */


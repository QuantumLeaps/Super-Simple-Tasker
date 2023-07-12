/**
 * @file system_dspic33ep128gs804.c
 * @author ASHRAF (ashrafkamel491@gmail.com)
 * @version 1 
 */

#include "system_dspic33ep128gs804.h"
#include "xc.h"

/**
 * Sets the output of a GPIO pin .
 * @param[in] GPIOx: where x can be (A,B,C) to select the GPIO peripheral.
 * @param pin_no: The port pin to be set to a certain output level.
 *                This parameter can be one of PIN_X where x can be (0..15) or ALL.
 * @param level: the new output state of the pin
 */
void gpio_set_level(gpio_t gpio, gpio_pin_t pin_num, int8_t state)
{
    if (state)
        gpio->PORT |= pin_num;
    else
        gpio->PORT &= ~pin_num;
}

/**
  * Gets the input of a GPIO pin .
  * @param[in] GPIOx: where x can be (A,B,C) to select the GPIO peripheral.
  * @param  pin_no: The port pin to be read.
  * This parameter can be one of PIN_X where x can be (0..15) or ALL.
  * @return -HIGH 1
  *         -LOW  0
  */
int8_t gpio_get_level(gpio_t gpio,gpio_pin_t pin_num)
{
    return (gpio->PORT & pin_num) ? HIGH : LOW;
}

/**
 * Sets the direction of a specific pin
 * @param[in] GPIOx: where x can be (A,B,C) to select the GPIO peripheral.
 * @param pin: The port pin to be assigned a direction.
 * This parameter can be one of PIN_X where x can be (0..15) or ALL.
 * @param direction: is the new direction of the pin 
 * 
 * \b Example:
 * @code
 * gpio_set_direction(GPIOA, PIN_8, GPIO_INPUT_PU);
 * @endcode
 */
void gpio_set_direction(gpio_t GPIOx, gpio_pin_t pin_no, gpio_direction_t direction)
{
    if(direction == GPIO_INPUT_AN)
    {
        GPIOx->TRIS  |= pin_no;   //set direction input
        GPIOx->CNEN &= ~pin_no;    //disable change int
        GPIOx->CNPU &= ~pin_no;    //disable pull up
        GPIOx->CNPD &= ~pin_no;    //disable pull down
        GPIOx->ANSEL |= pin_no;
    }
    else if (direction == GPIO_OPEN_DRAIN)
    {
        GPIOx->ODC |= pin_no;
        GPIOx->ANSEL &= ~pin_no;
    }
    else
    {
        GPIOx->ODC &= ~pin_no;
        GPIOx->ANSEL &= ~pin_no;
        if (direction == GPIO_OUTPUT)
        {
            GPIOx->CNEN &= ~pin_no;    //disable change int
            GPIOx->CNPU &= ~pin_no;    //disable pull up
            GPIOx->CNPD &= ~pin_no;    //disable pull down
            GPIOx->TRIS &= ~pin_no;    //set direction output
        }
        else
        {
            GPIOx->TRIS  |= pin_no;   //set direction input

            if (direction == (GPIO_INPUT_PU))
            {
                GPIOx->CNPD  &= ~pin_no;
                GPIOx->CNPU  |= pin_no;
            }
            else if (direction == (GPIO_INPUT_PD))
            {
                GPIOx->CNPU  &= ~pin_no;
                GPIOx->CNPD  |= pin_no;
            }
        }
    }
    
}

#define F_CPU_MHZ   7.37F

void SysTick_Config(uint16_t Freq_hz) 
{
    
    float period_us = 1.0/(float)Freq_hz;
    period_us *= 1000;
    period_us *= 1000;
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b01; // Select 1:8 Prescaler
    TMR1 = 0x00; // Clear timer register
    volatile uint16_t temp = F_CPU_MHZ * period_us / 16; // Load the period value
    PR1 = temp;
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}


void SystemCoreClockUpdate(void)
{
    /*doze divider configure*/
    CLKDIVbits.DOZE = 0b010;
    // use default internal Fast RC oscillator
    // 7.37 MHz
}

void dspic_SystemReset(void)
{
    __builtin_software_breakpoint();
}

#define IFS_BASE     ((uint16_t volatile *)0x800U)  //interrupt flag Status (pend)
#define IEC_BASE     ((uint16_t volatile *)0x820U)  //interrupt enable control
#define IPC_BASE     ((uint16_t volatile *)0x840U)  //interrupt priority control

void interrupt_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
    //CLR
    IPC_BASE[IRQn/4] &= ~(0X7<< ((IRQn%4)*4));
    //SET
    IPC_BASE[IRQn/4] |= priority << ((IRQn%4)*4);
}

void interrupt_Enable(IRQn_Type IRQn)
{
    IEC_BASE[IRQn/16] |= 1 << (IRQn%16);
}


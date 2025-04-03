#include "gesture.h"
#include "stm32f0xx.h"

void init_gesture_sensor() {
    // I2C and APDS-9960 configuration code for fast mode here
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOB->AFR[0] |= (1 << (4 * 6)) | (1 << (4 * 7));

    I2C1->TIMINGR = 0x00300F38;



}

Direction get_gesture_direction() {
    // Read from APDS-9960 registers and interpret gesture
    return DIR_NONE;
}

#include "gesture.h"
#include "stm32f0xx.h"

#define APDS9960_I2C_ADDR 0x39

// ----- Low-level I2C -----
void i2c_write(uint8_t reg, uint8_t data) {
    I2C1->CR2 = (APDS9960_I2C_ADDR << 1) | (2 << 16);
    I2C1->CR2 |= I2C_CR2_START;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = reg;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = data;
    while (!(I2C1->ISR & I2C_ISR_TC));
    I2C1->CR2 |= I2C_CR2_STOP;
}

uint8_t i2c_read(uint8_t reg) {
    I2C1->CR2 = (APDS9960_I2C_ADDR << 1) | (1 << 16);
    I2C1->CR2 |= I2C_CR2_START;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = reg;
    while (!(I2C1->ISR & I2C_ISR_TC));

    I2C1->CR2 = (APDS9960_I2C_ADDR << 1) | (1 << 16) | I2C_CR2_RD_WRN;
    I2C1->CR2 |= I2C_CR2_START;
    while (!(I2C1->ISR & I2C_ISR_RXNE));
    uint8_t val = I2C1->RXDR;
    while (!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;
    return val;
}

// ----- Init Sensor -----
void init_gesture_sensor() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOB->AFR[0] |= (1 << (4 * 6)) | (1 << (4 * 7));
    GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;

    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->TIMINGR = 0x00300F38; // 400kHz
    I2C1->CR1 |= I2C_CR1_PE;

    uint8_t id = i2c_read(0x92);
    if (id != 0xAB) return;

    i2c_write(0x80, 0x41); // ENABLE: PON + GEN
    i2c_write(0xAD, 0x01); // starts gesture
}

Direction get_gesture_direction() {
    uint8_t gstatus = i2c_read(0xAF);
    if ((gstatus & 0x01) == 0) return DIR_NONE;

    uint8_t gflvl = i2c_read(0xAE);
    if (gflvl == 0) return DIR_NONE;

    i2c_read(0xFC); //up
    i2c_read(0xFC); //down
    uint8_t left = i2c_read(0xFC); //left
    uint8_t right = i2c_read(0xFC); //right

    if (left > right + 10)
        return DIR_LEFT;
    else if (right > left + 10)
        return DIR_RIGHT;

    return DIR_NONE;
}

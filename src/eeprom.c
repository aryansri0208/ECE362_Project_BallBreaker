#include "eeprom.h"
#include "stm32f0xx.h"
#include <stdint.h>
void enable_ports() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
    GPIOA->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);
    GPIOA->AFR[1] &= ~((0xF << 12) | (0xF << 16));
    GPIOA->AFR[1] |= ((0x4 << 12) | (0x4 << 16));
}

void init_i2c() {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_ANFOFF;
    I2C1->CR1 |= I2C_CR1_ERRIE;
    I2C1->CR1 |= I2C_CR1_NOSTRETCH;

    I2C1->TIMINGR = 0x00B01A4B; //(0x5<<28) | (0x3<<20) | (0x3<<16) | (0x3<<8) | (0x9<<0);

    I2C1->CR2 &= ~I2C_CR2_ADD10;

    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_start(uint8_t targadr, uint8_t size, uint8_t dir) {
    uint32_t tmpreg = I2C1->CR2;

    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
    tmpreg |= ((dir << 10) & I2C_CR2_RD_WRN);
    tmpreg |= ((targadr << 1) & I2C_CR2_SADD) | ((size << 16) & I2C_CR2_NBYTES);
    tmpreg |= I2C_CR2_START;
    I2C1->CR2 = tmpreg;
}

void i2c_stop(void) {
    if ((I2C1->CR2 & I2C_CR2_STOP) != 0) return;

    I2C1->CR2 |= I2C_CR2_STOP;

    while ((I2C1->ISR & I2C_ISR_STOPF) == 0) {}

    I2C1->ICR |= I2C_ICR_STOPCF; // Clear STOPF
}

// Step 3.2: i2c_waitidle
void i2c_waitidle(void) {
    while ((I2C1->ISR & I2C_ISR_BUSY) != 0) {}
}

// Step 3.3: i2c_senddata
int i2c_senddata(uint8_t addr, uint8_t *data, uint8_t size) {
    i2c_waitidle();
    i2c_start(addr, size, 0); // Write

    for (int i = 0; i < size; i++) {
        int count = 0;
        while ((I2C1->ISR & I2C_ISR_TXIS) == 0) {
            count++;
            if (count > 1000000) return -1;
            if (i2c_checknack()) {
                i2c_clearnack();
                i2c_stop();
                return -1;
            }
        }
        I2C1->TXDR = data[i] & I2C_TXDR_TXDATA;
    }

    while (((I2C1->ISR & I2C_ISR_TC) == 0) && ((I2C1->ISR & I2C_ISR_NACKF) == 0)) {}

    if (I2C1->ISR & I2C_ISR_NACKF) {
        return -1;
    }

    i2c_stop();
    return 0;
}

// Step 3.4: i2c_recvdata
int i2c_recvdata(uint8_t addr, uint8_t *data, uint8_t size) {
    i2c_waitidle();
    i2c_start(addr, size, 1); // Read

    for (int i = 0; i < size; i++) {
        int count = 0;
        while ((I2C1->ISR & I2C_ISR_RXNE) == 0) {
            count++;
            if (count > 1000000) return -1;
            if (i2c_checknack()) {
                i2c_clearnack();
                i2c_stop();
                return -1;
            }
        }
        data[i] = I2C1->RXDR & I2C_RXDR_RXDATA;
    }

    i2c_stop();
    return 0;
}

// Step 3.5: i2c_clearnack
void i2c_clearnack(void) {
    I2C1->ICR |= I2C_ICR_NACKCF;
}

// Step 3.6: i2c_checknack
int i2c_checknack(void) {
    return (I2C1->ISR & I2C_ISR_NACKF) != 0;
}

// Step 4: Demonstrate data read and write for High Score
void save_high_score(uint8_t score) {
    uint8_t data[2] = {0x00, score}; // EEPROM address 0x00, then score
    i2c_senddata(0x57, data, 2);
}

uint8_t read_high_score(void) {
    uint8_t addr = 0x00;
    uint8_t score;
    i2c_senddata(0x57, &addr, 1); // Set EEPROM read pointer
    i2c_recvdata(0x57, &score, 1);
    return score;
}

// Example main for testing (or integrate into Brick Breaker logic)
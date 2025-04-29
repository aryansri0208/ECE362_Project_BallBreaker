#include "gesture.h"
#include "stm32f0xx.h"

#define APDS9960_I2C_ADDR 0x39



void enable_ports_gesture() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER11 | GPIO_MODER_MODER12);
    GPIOA->MODER |= (GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1);
    GPIOA->AFR[1] &= ~((0xF << 12) | (0xF << 16));
    GPIOA->AFR[1] |= ((0x5 << 12) | (0x5 << 16));

    // GPIOA->OTYPER |= GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_12;
    // GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);
    // GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_0);
}

void init_i2c() {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    I2C2->CR1 &= ~I2C_CR1_PE;
    I2C2->CR1 |= I2C_CR1_ANFOFF;
    I2C2->CR1 |= I2C_CR1_ERRIE;
    I2C2->CR1 |= I2C_CR1_NOSTRETCH;

    I2C2->TIMINGR = 0;
    I2C2->TIMINGR = ~I2C_TIMINGR_PRESC;
    I2C2->TIMINGR = (0x5<<28) | (0x3<<20) | (0x1<<16) | (0x3<<8) | (0x9<<0);

    I2C2->CR2 &= ~I2C_CR2_ADD10;

    I2C2->CR1 |= I2C_CR1_PE;
}

// i2c_start
void i2c_start_gesture(uint8_t targadr, uint8_t size, uint8_t dir) {
    uint32_t tmpreg = I2C2->CR2;

    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
    tmpreg |= ((dir << 10) & I2C_CR2_RD_WRN);
    tmpreg |= ((targadr << 1) & I2C_CR2_SADD) | ((size << 16) & I2C_CR2_NBYTES);
    tmpreg |= I2C_CR2_START;
    I2C2->CR2 = tmpreg;
}

void i2c_stop_gesture(void) {
    if ((I2C2->CR2 & I2C_CR2_STOP) != 0) return;

    I2C2->CR2 |= I2C_CR2_STOP;

    while ((I2C2->ISR & I2C_ISR_STOPF) == 0) {}

    I2C2->ICR |= I2C_ICR_STOPCF; // Clear STOPF
}

// Step 3.2: i2c_waitidle
void i2c_waitidle_gesture(void) {
    while ((I2C2->ISR & I2C_ISR_BUSY) != 0) {}
}

// Step 3.3: i2c_senddata
int i2c_senddata_gesture(uint8_t addr, uint8_t *data, uint8_t size) {
    i2c_waitidle_gesture();
    i2c_start_gesture(addr, size, 0); // Write

    for (int i = 0; i < size; i++) {
        int count = 0;
        while ((I2C2->ISR & I2C_ISR_TXIS) == 0) {
            count++;
            if (count > 1000000) {
                return -1;
            }
            if (i2c_checknack_gesture()) {
                i2c_clearnack_gesture();
                i2c_stop_gesture();
                return -1;
            }
        }
        I2C2->TXDR = data[i] & I2C_TXDR_TXDATA;
    }

    while (((I2C2->ISR & I2C_ISR_TC) == 0) && ((I2C2->ISR & I2C_ISR_NACKF) == 0)) {}

    if (I2C2->ISR & I2C_ISR_NACKF) {
        return -1;
    }

    i2c_stop_gesture();
    return 0;
}

// Step 3.4: i2c_recvdata
int i2c_recvdata_gesture(uint8_t addr, uint8_t *data, uint8_t size) {
    i2c_waitidle_gesture();
    i2c_start_gesture(addr, size, 1); // Read

    for (int i = 0; i < size; i++) {
        int count = 0;
        while ((I2C2->ISR & I2C_ISR_RXNE) == 0) {
            count++;
            if (count > 1000000) return -1;
            if (i2c_checknack_gesture()) {
                i2c_clearnack_gesture();
                i2c_stop_gesture();
                return -1;
            }
        }
        data[i] = I2C2->RXDR & I2C_RXDR_RXDATA;
    }

    i2c_stop_gesture();
    return 0;
}

// Step 3.5: i2c_clearnack
void i2c_clearnack_gesture(void) {
    I2C2->ICR |= I2C_ICR_NACKCF;
}

// Step 3.6: i2c_checknack
int i2c_checknack_gesture(void) {
    return (I2C2->ISR & I2C_ISR_NACKF) != 0;
}

/*// Step 4: Demonstrate data read and write for High Score
void save_high_score(uint8_t score) {
    char temp[3];
    snprintf(temp, 3, "%d", score);
    char score_byte = score;
    eeprom_write(0x0200, temp, 3);  // Store at EEPROM address 0x0000
}

uint8_t read_high_score(void) {
    char * score_byte;
    char temp[3] = {0};
    eeprom_read(0x0200, temp, 3);  // Read from EEPROM address 0x0000
    uint8_t x = strtol(temp, &score_byte, 10);
    return x;
}*/


void gesture_write(uint16_t loc, const char* data, uint8_t len) {
    uint8_t bytes[34];
    bytes[0] = loc>>8;
    bytes[1] = loc&0xFF;
    for(int i = 0; i<len; i++){
        bytes[i+2] = data[i];
    }
    i2c_senddata_gesture(APDS9960_I2C_ADDR, bytes, len+2);
}

void gesture_read(uint16_t loc, char data[], uint8_t len) {
    // ... your code here
    uint8_t bytes[2];
    bytes[0] = loc>>8;
    bytes[1] = loc&0xFF;
    i2c_senddata_gesture(APDS9960_I2C_ADDR, bytes, 2);
    i2c_recvdata_gesture(APDS9960_I2C_ADDR, data, len);
}

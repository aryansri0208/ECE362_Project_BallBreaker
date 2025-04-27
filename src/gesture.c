// #include "gesture.h"
// #include "stm32f0xx.h"

// #define APDS9960_I2C_ADDR 0x39

// // ----- Low-level I2C -----
// void i2c_write(uint8_t reg, uint8_t data) {
//     I2C2->CR2 = (APDS9960_I2C_ADDR << 1) | (2 << 16);
//     i2c_start(0x39, 2, 0);
//     //I2C2->CR2 |= I2C_CR2_START;
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
//     I2C2->TXDR = reg;
//     // while (!(I2C2->ISR & I2C_ISR_TXIS));
//     // I2C2->TXDR = data;
//     /*while (!(I2C2->ISR & I2C_ISR_TC));
//     I2C2->CR2 |= I2C_CR2_STOP;*/
// }

// uint8_t i2c_read(uint8_t reg) {
//     I2C2->CR2 = (APDS9960_I2C_ADDR << 1) | (1 << 16);
//     I2C2->CR2 |= I2C_CR2_START;
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
//     I2C2->TXDR = reg;
//     while (!(I2C2->ISR & I2C_ISR_TC));

//     I2C2->CR2 = (APDS9960_I2C_ADDR << 1) | (1 << 16) | I2C_CR2_RD_WRN;
//     I2C2->CR2 |= I2C_CR2_START;
//     while (!(I2C2->ISR & I2C_ISR_RXNE));
//     uint8_t val = I2C2->RXDR;
//     while (!(I2C2->ISR & I2C_ISR_STOPF));
//     I2C2->ICR |= I2C_ICR_STOPCF;
//     return val;
// }

// void enable_ports() {
//     RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

//     GPIOA->MODER &= ~(GPIO_MODER_MODER11 | GPIO_MODER_MODER12);
//     GPIOA->MODER |= (GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1);
//     GPIOA->AFR[1] &= ~((0xF << 12) | (0xF << 16));
//     GPIOA->AFR[1] |= ((0x5 << 12) | (0x5 << 16));

//     // GPIOA->OTYPER |= GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_12;
//     // GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);
//     // GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_0);
// }

// void init_i2c() {
//     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
//     I2C2->CR1 &= ~I2C_CR1_PE;
//     I2C2->CR1 |= I2C_CR1_ANFOFF;
//     I2C2->CR1 |= I2C_CR1_ERRIE;
//     I2C2->CR1 |= I2C_CR1_NOSTRETCH;

//     I2C2->TIMINGR = 0x00B01A4B; //(0x5<<28) | (0x3<<20) | (0x3<<16) | (0x3<<8) | (0x9<<0);

//     I2C2->CR2 &= ~I2C_CR2_ADD10;

//     I2C2->CR1 |= I2C_CR1_PE;
// }

// // i2c_start
// void i2c_start(uint8_t targadr, uint8_t size, uint8_t dir) {
//     uint32_t tmpreg = I2C2->CR2;

//     tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
//     tmpreg |= ((dir << 10) & I2C_CR2_RD_WRN);
//     tmpreg |= ((targadr << 1) & I2C_CR2_SADD) | ((size << 16) & I2C_CR2_NBYTES);
//     tmpreg |= I2C_CR2_START;
//     I2C2->CR2 = tmpreg;
// }

// void i2c_stop() {
//     if (I2C2->ISR & I2C_ISR_STOPF) {
//         return;  
//     }

//     I2C2->CR2 |= I2C_CR2_STOP;
//     while (!(I2C2->ISR & I2C_ISR_STOPF)) {
//     }
//     I2C2->ICR |= I2C_ICR_STOPCF;
// }




// void i2c_waitidle() {
//     while (I2C2->ISR & I2C_ISR_BUSY) {
//     }
// }

// /*int i2c_senddata(uint8_t targadr, uint8_t *data, uint8_t size) {
//     int count;


//     i2c_waitidle();

//     i2c_start(targadr, size, 0);  

//     for (int i = 0; i < size; i++) {
//         count = 0;

//         while ((I2C2->ISR & I2C_ISR_TXIS) == 0) {
//             count += 1;
//             if (count > 1000000) {  
//                 return -1;  
//             }

//             if (i2c_checknack()) {
//                 i2c_clearnack();
//                 i2c_stop();
//                 return -1;
//             }
//         }

//         I2C2->TXDR = data[i];
//     }

//     while (!(I2C2->ISR & I2C_ISR_TC)) {
//         if (I2C2->ISR & I2C_ISR_NACKF) {
//             return -1;  
//         }
//     }

//     i2c_stop();
//     return 0;
// }


// int i2c_recvdata(uint8_t targadr, uint8_t *data, uint8_t size) {
//     int count;

//     i2c_waitidle();
//     i2c_start(targadr, size, 1); 
//     for (int i = 0; i < size; i++) {
//         count = 0;

//         while ((I2C2->ISR & I2C_ISR_RXNE) == 0) {
//             count += 1;
//             if (count > 1000000) { 
//                 return -1; 
//             }


//             if (i2c_checknack()) {
//                 i2c_clearnack();
//                 i2c_stop();
//                 return -1; 
//             }
//         }
//         data[i] = I2C2->RXDR & I2C_RXDR_RXDATA;
//     }

//     while (!(I2C2->ISR & I2C_ISR_TC)) {
//         if (I2C2->ISR & I2C_ISR_NACKF) {
//             return -1;  
//         }
//     }
//     i2c_stop();
//     return 0;
// }
// */


// Direction get_gesture_direction() {
//     uint8_t gstatus = i2c_read(0xAF);
//     if ((gstatus & 0x01) == 0) return DIR_NONE;

//     uint8_t gflvl = i2c_read(0xAE);
//     if (gflvl == 0) return DIR_NONE;

//     i2c_read(0xFC); //up
//     i2c_read(0xFC); //down
//     uint8_t left = i2c_read(0xFC); //left
//     uint8_t right = i2c_read(0xFC); //right

//     if (left > right + 10)
//         return DIR_LEFT;
//     else if (right > left + 10)
//         return DIR_RIGHT;

//     return DIR_NONE;
// }

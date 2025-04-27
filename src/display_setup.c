#include "display_setup.h"
#include "display_game_function.h"
#include "lcd.h"
#include "stm32f0xx.h"
#include <string.h>
#include <stdio.h>
#include "lcd.h"

void init_spi1_slow() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    //PB3=SCK, PB4=MISO, PB5=MOSI 
    GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
    GPIOB->MODER |=  (GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1); //alternate function

    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | GPIO_AFRL_AFSEL4 | GPIO_AFRL_AFSEL5); 

    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2);
    SPI1->CR2 &= ~(SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3);
    SPI1->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM;
    //SPI1->CR2 |= SPI_CR2_TXDMAEN;
    //SPI1->CR1 |= SPI_CR1_DFF;

    SPI1->CR2 |= SPI_CR2_FRXTH;
    SPI1->CR1 |= SPI_CR1_SPE;     
    
}

void init_lcd_spi() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER11 | GPIO_MODER_MODER14);
    GPIOB->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER14_0);
    init_spi1_slow();
    sdcard_io_high_speed();

}

void sdcard_io_high_speed() {
    SPI1->CR1 &= ~SPI_CR1_SPE;

    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_0;

    SPI1->CR1 |= SPI_CR1_SPE;
}

int display_main(void) {
    LCD_Setup();  
    setup_game_screen();
}

void init_buttons(void) {
    // Enable clock for GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Set PA1, PA2, PA3 as input (00)
    GPIOA->MODER &= ~(0xF << (2 * 1)); // Clear bits for PA1, PA2
    GPIOA->MODER &= ~(0x3 << (2 * 3)); // Clear bits for PA3

    // Set PA1, PA2, PA3 pull-up (01)
    GPIOA->PUPDR &= ~(0xF << (2 * 1)); // Clear pull settings
    GPIOA->PUPDR &= ~(0x3 << (2 * 3));
    
    GPIOA->PUPDR |= (0x5 << (2 * 1));  // PA1 and PA2 pull-up
    GPIOA->PUPDR |= (0x1 << (2 * 3));  // PA3 pull-up
}


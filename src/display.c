#include "display.h"
#include "lcd.h"
#include "stm32f0xx.h"
#include <string.h>
#include <stdio.h>

// Paddle
#define PADDLE_WIDTH   40
#define PADDLE_HEIGHT  6
int paddle_x = 100;
int paddle_y = 300;

// Ball
int ball_x = 120;
int ball_y = 290;
int ball_radius = 4;
int ball_vx = 1;
int ball_vy = -1;

// Bricks
#define BRICK_ROWS 4
#define BRICK_COLS 6
#define BRICK_WIDTH  35
#define BRICK_HEIGHT 12
int bricks[BRICK_ROWS][BRICK_COLS];  //1-active, 0-destroyed

// Score
int score = 0;

void init_display(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    //PB3=SCK, PB4=MISO, PB5=MOSI 
    GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
    GPIOB->MODER |=  (GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1); //alternate function

    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3 | GPIO_AFRL_AFSEL4 | GPIO_AFRL_AFSEL5); 

    SPI2->CR1 &= ~SPI_CR1_SPE;
    SPI2->CR1 |= SPI_CR1_BR;
    SPI2->CR2 |= SPI_CR2_DS;
    SPI2->CR1 |= SPI_CR1_MSTR;
    SPI2->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP;
    SPI2->CR2 |= SPI_CR2_TXDMAEN;
    SPI2->CR1 |= SPI_CR1_SPE;     
}



void draw_score(void) {
    char buffer[16];
    sprintf(buffer, "Score: %d", score);
    LCD_ShowString(10, 2, buffer, RED, BLACK, 12, 0);
}

void draw_background(void) {
    LCD_Clear(BLACK);
}

void draw_paddle(void) {
    LCD_DrawFillRectangle(
        paddle_x,
        paddle_y,
        paddle_x + PADDLE_WIDTH,
        paddle_y + PADDLE_HEIGHT,
        WHITE
    );
}

void draw_ball(void) {
    LCD_Circle(ball_x, ball_y, ball_radius, 1, GREEN);
}

void draw_bricks(void) {
    int x_offset = 10;
    int y_offset = 20;
    int spacing = 5;

    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            if (bricks[row][col]) {
                int x0 = x_offset + col * (BRICK_WIDTH + spacing);
                int y0 = y_offset + row * (BRICK_HEIGHT + spacing);
                LCD_DrawFillRectangle(x0, y0, x0 + BRICK_WIDTH, y0 + BRICK_HEIGHT, RED);
            }
        }
    }
}

void init_game_state(void) {
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            bricks[row][col] = 1;
        }
    }
    score = 0;
    paddle_x = 100;
    ball_x = 120;
    ball_y = 290;
    ball_vx = 1;
    ball_vy = -1;
}

void move_ball_and_check_collision(void) {
    //move the ball
    ball_x += ball_vx;
    ball_y += ball_vy;

    //runs into wall
    if (ball_x - ball_radius <= 0 || ball_x + ball_radius >= SCREEN_WIDTH)
        ball_vx = -ball_vx;
    if (ball_y - ball_radius <= 0)
        ball_vy = -ball_vy;

    //hits paddle
    if (ball_y + ball_radius >= paddle_y &&
        ball_y + ball_radius <= paddle_y + PADDLE_HEIGHT &&
        ball_x >= paddle_x && ball_x <= paddle_x + PADDLE_WIDTH) {
        ball_vy = -ball_vy;
    }

    //hits bricks
    int x_offset = 10;
    int y_offset = 20;
    int spacing = 5;
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            if (!bricks[row][col]) continue;

            int x0 = x_offset + col * (BRICK_WIDTH + spacing);
            int y0 = y_offset + row * (BRICK_HEIGHT + spacing);
            int x1 = x0 + BRICK_WIDTH;
            int y1 = y0 + BRICK_HEIGHT;

            if (ball_x >= x0 && ball_x <= x1 && ball_y >= y0 && ball_y <= y1) {
                bricks[row][col] = 0;
                score += 1;
                ball_vy = -ball_vy;
                return;
            }
        }
    }
}

void draw_game_frame(void) {
    draw_background();
    draw_bricks();
    draw_paddle();
    draw_ball();
    draw_score();
}

void display_game_over(void) {
    LCD_ShowString(60, 160, "GAME OVER", RED, BLACK, 16, 1);
}

int display_main(void) {
    LCD_Setup();           
    init_game_state();     
    draw_game_frame();    

    while (1) {
        nano_wait(1000000);  

        move_ball_and_check_collision();
        draw_game_frame();

        if (ball_y - 4 > SCREEN_HEIGHT) {
            display_game_over();
            break;
        }
    }

    while (1);  
}
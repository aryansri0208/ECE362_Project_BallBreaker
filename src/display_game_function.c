#include "display_setup.h"
#include "display_game_function.h"
#include "lcd.h"
#include "stm32f0xx.h"
#include <string.h>
#include <stdio.h>

// Paddle dimensions and starting position
#define PADDLE_WIDTH   40
#define PADDLE_HEIGHT  6
int paddle_x = 100;
int paddle_y = 300;

// Ball dimensions (we won't move the ball for now)
int ball_x = 120;
int ball_y = 290;
int ball_radius = 4;

// Only drawing and movement for now

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

void erase_paddle(void) {
    LCD_DrawFillRectangle(
        paddle_x,
        paddle_y,
        paddle_x + PADDLE_WIDTH,
        paddle_y + PADDLE_HEIGHT,
        BLACK
    );
}

void draw_bricks(void) {
    int brick_width = 30;
    int brick_height = 10;
    int start_x = 20;
    int start_y = 20;
    int gap = 5;

    u16 row_colors[3] = { RED, GREEN, BLUE };

    for (int row = 0; row < 3; row++) {
        for (int i = 0; i < 6; i++) {
            int x1 = start_x + i * (brick_width + gap);
            int y1 = start_y + row * (brick_height + gap);
            LCD_DrawFillRectangle(x1, y1, x1 + brick_width, y1 + brick_height, row_colors[row]);
        }
    }
}

void draw_ball(void) {
    LCD_Circle(ball_x, ball_y, ball_radius, 1, WHITE);
}

void display_high_score(void) {
    draw_background();
    uint16_t score = 20;  // placeholder
    
    LCD_DrawFillRectangle(20, 130, 220, 190, WHITE);
    
    char score_text[30];
    sprintf(score_text, "Prev High Score: %d", score);
    LCD_DrawString(50, 140, BLACK, WHITE, score_text, 16, 0);
    
    for (volatile int i = 0; i < 3000000; i++);
    draw_background();
}

void setup_game_screen(void) {
    display_high_score();
    draw_background();
    draw_bricks();
    draw_paddle();
    draw_ball();
}

// Movement functions
void move_paddle_left(void) {
    if (paddle_x > 0) {
        erase_paddle();
        paddle_x -= 5;
        draw_paddle();
    }
}

void move_paddle_right(void) {
    if (paddle_x + PADDLE_WIDTH < LCD_W) {
        erase_paddle();
        paddle_x += 5;
        draw_paddle();
    }
}

// Button read
int is_button_pressed(int button_num) {
    if (button_num == 1) {
        return (GPIOA->IDR & (1 << 1)) == 0; // PA1
    } else if (button_num == 2) {
        return (GPIOA->IDR & (1 << 2)) == 0; // PA2
    }
    return 0;
}

// Button setup
void init_buttons(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER &= ~(0x3 << (2 * 1)); // PA1 input
    GPIOA->MODER &= ~(0x3 << (2 * 2)); // PA2 input

    GPIOA->PUPDR &= ~(0x3 << (2 * 1));
    GPIOA->PUPDR &= ~(0x3 << (2 * 2));

    GPIOA->PUPDR |= (0x1 << (2 * 1));  // pull-up PA1
    GPIOA->PUPDR |= (0x1 << (2 * 2));  // pull-up PA2
}

// Main game function
void play_game(void) {
    init_buttons();
    setup_game_screen();

    while (1) {
        if (is_button_pressed(1)) {
            move_paddle_left();
        }
        if (is_button_pressed(2)) {
            move_paddle_right();
        }
        for (volatile int i = 0; i < 50000; i++); // small delay
    }
}

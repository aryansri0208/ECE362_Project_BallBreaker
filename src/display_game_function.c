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

// Ball dimensions
int ball_x = 120;
int ball_y = 290;
int ball_radius = 4;
/*
// Simple EEPROM read stub
uint16_t eeprom_read_high_score(void) {
    // TODO: Replace with your EEPROM reading function
    return 123;  // Placeholder high score
}*/

// Draw full black background
void draw_background(void) {
    LCD_Clear(BLACK);
}

// Draw paddle
void draw_paddle(void) {
    LCD_DrawFillRectangle(
        paddle_x,
        paddle_y,
        paddle_x + PADDLE_WIDTH,
        paddle_y + PADDLE_HEIGHT,
        WHITE
    );
}

void draw_bricks(void) {
    int brick_width = 30;
    int brick_height = 10;
    int start_x = 20;
    int start_y = 20;
    int gap = 5;

    u16 row_colors[3] = { RED, GREEN, BLUE }; // Different color for each row

    for (int row = 0; row < 3; row++) {
        for (int i = 0; i < 6; i++) {
            int x1 = start_x + i * (brick_width + gap);
            int y1 = start_y + row * (brick_height + gap);
            LCD_DrawFillRectangle(x1, y1, x1 + brick_width, y1 + brick_height, row_colors[row]);
        }
    }
}

// Draw ball (as a filled circle)
void draw_ball(void) {
    LCD_Circle(ball_x, ball_y, ball_radius, 1, WHITE);
}
/*
// Display high score on black screen for 3 seconds
void display_high_score(void) {
    draw_background();
    uint16_t score = eeprom_read_high_score();
    
    // Draw a white box
    int box_x1 = 60;
    int box_y1 = 130;
    int box_x2 = 180;
    int box_y2 = 190;
    LCD_DrawFillRectangle(box_x1, box_y1, box_x2, box_y2, WHITE);
    
    // Print the score inside
    char score_text[20];
    sprintf(score_text, "High: %d", score);
    LCD_DrawString(70, 150, BLACK, WHITE, score_text, 16, 0);
    
    // Delay for 3 seconds (replace with proper delay if needed)
    for (volatile int i = 0; i < 3000000; i++);
}
*/
// Setup initial game screen
void setup_game_screen(void) {
    //draw_background();
    draw_paddle();
    draw_bricks();
    draw_ball();
}

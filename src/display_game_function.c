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
int ball_moving = 0;  // 0 = stationary, 1 = moving
int ball_speed_y = -2; // ball moves up when launched

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

// Draw bricks
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

// Display high score on black screen for 3 seconds
void display_high_score(void) {
    draw_background();
    uint16_t score = 20;  // placeholder
    
    // Draw a white box
    int box_x1 = 20;
    int box_y1 = 130;
    int box_x2 = 220;
    int box_y2 = 190;
    LCD_DrawFillRectangle(box_x1, box_y1, box_x2, box_y2, WHITE);
    
    // Print the score inside
    char score_text[30];
    sprintf(score_text, "Prev High Score: %d", score);
    LCD_DrawString(50, 140, BLACK, WHITE, score_text, 16, 0);
    
    // Delay for 3 seconds (replace with proper delay if needed)
    for (volatile int i = 0; i < 3000000; i++);
    draw_background();
}

// Setup initial game screen
void setup_game_screen(void) {
    display_high_score();
    draw_paddle();
    draw_bricks();
    draw_ball();
}

// -----------------------------------------------
// Movement and Game Functions
// -----------------------------------------------

// Move paddle left
void move_paddle_left(void) {
    if (paddle_x > 0) {
        paddle_x -= 5;
    }
}

// Move paddle right
void move_paddle_right(void) {
    if (paddle_x + PADDLE_WIDTH < LCD_W) {
        paddle_x += 5;
    }
}

// Launch the ball
void launch_ball(void) {
    if (ball_moving == 0) {
        ball_moving = 1;
    }
}

// Update ball position
void update_ball(void) {
    if (ball_moving) {
        ball_y += ball_speed_y;
    }
}

// Read button states
int is_button_pressed(int button_num) {
    if (button_num == 1) {
        return (GPIOA->IDR & (1 << 1)) == 0; // PA1
    } else if (button_num == 2) {
        return (GPIOA->IDR & (1 << 2)) == 0; // PA2
    } else if (button_num == 3) {
        return (GPIOA->IDR & (1 << 3)) == 0; // PA3
    }
    return 0;
}


// Redraw the paddle and ball without full background clearing (faster)
void redraw_moving_objects(void) {
    // Erase old paddle and ball
    LCD_DrawFillRectangle(0, paddle_y, LCD_W, paddle_y + PADDLE_HEIGHT, BLACK);
    LCD_DrawFillRectangle(0, 0, LCD_W, 100, BLACK); // just erase top 100 px (ball area)

    draw_paddle();
    draw_ball();
}

// Game loop
void game_loop(void) {
    while (1) {
        if (is_button_pressed(1)) {
            move_paddle_left();
        }
        if (is_button_pressed(2)) {
            move_paddle_right();
        }
        if (is_button_pressed(3)) {
            launch_ball();
        }

        update_ball();
        redraw_moving_objects();

        for (volatile int i = 0; i < 50000; i++); // small delay
    }
}

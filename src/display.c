#include "display.h"
#include "stm32f0xx.h"
#include <stdbool.h>


void init_display() {
    // SPI init for TFT
}

void update_paddle(Direction dir) {
    // Move paddle left/right based on gesture input
}

bool update_ball_position() {
    // Ball physics and collision detection
    return false;
}

void update_display(int score, int high_score) {
    // Draw score, paddle, ball, and bricks
}

bool check_game_over() {
    // Determine if ball hit bottom edge
    return false;
}

void show_game_over_screen(int score, int high_score) {
    // Game over rendering
}

void wait_for_restart() {
    // Poll for restart condition (e.g., button press)
}
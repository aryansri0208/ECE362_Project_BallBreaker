#ifndef DISPLAY_GAME_FUNCTION_H
#define DISPLAY_GAME_FUNCTION_H

void draw_score();
void draw_background();
void draw_paddle();
void draw_ball();
void draw_bricks();
void init_game_state();
void move_ball_and_check_collision();
void draw_game_frame();
void display_game_over();

#endif
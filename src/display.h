#ifndef DISPLAY_H
#define DISPLAY_H

#include "gesture.h"

void init_display();
void update_paddle(Direction dir);
bool update_ball_position();
void update_display(int score, int high_score);
bool check_game_over();
void show_game_over_screen(int score, int high_score);
void wait_for_restart();

#endif

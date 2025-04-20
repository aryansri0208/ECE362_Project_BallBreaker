#include "gesture.h"
#include "display.h"
#include "sound.h"
#include "eeprom.h"
#include <stdint.h>
#include <stdbool.h>
/*
void game_loop() {
    internal_clock();
    init_display();
    init_gesture_sensor();
    init_sound();
    init_eeprom();

    int high_score = read_high_score();
    int current_score = 0;

//     while (true) {
//         Direction dir = get_gesture_direction();
//         update_paddle(dir);
//         bool brick_hit = update_ball_position();

//         if (brick_hit) {
//             current_score++;
//             play_sound(SOUND_HIT);
//         }

//         update_display(current_score, high_score);

//         if (check_game_over()) {
//             if (current_score > high_score) {
//                 write_high_score(current_score);
//             }
//             show_game_over_screen(current_score, high_score);
//             wait_for_restart();
//             current_score = 0;
//         }
//     }
}*/

void internal_clock();

int main(void) {
    internal_clock();
    display_main();
    return 0;
}

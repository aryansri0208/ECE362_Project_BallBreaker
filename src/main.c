#include "gesture.h"
#include "display_setup.h"
#include "display_game_function.h"
#include "sound.h"
#include "eeprom.h"
#include <stdint.h>
#include <stdbool.h>

void internal_clock();



int main(void) {
    internal_clock();
    display_main();
    //play_game();

    enable_ports_gesture();
    init_i2c_gesture();
    apds9960_init();
    while (1){
        char x = detect_left_or_right_gesture();
    }
    delay_ms(200);
    return 0;
}

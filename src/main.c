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
    enable_ports();
    init_i2c();
    i2c_start(0x39, 0, 0);
    i2c_stop();
    //get_gesture_direction();
    return 0;
}

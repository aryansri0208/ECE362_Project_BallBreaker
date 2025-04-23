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
 //   init_gesture_sensor();
    get_gesture_direction();
    return 0;
}

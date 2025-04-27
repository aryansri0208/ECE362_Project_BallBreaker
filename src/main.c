#include "gesture.h"
#include "display_setup.h"
#include "display_game_function.h"
#include "sound.h"
#include "eeprom.h"
#include <stdint.h>
#include <stdbool.h>

void internal_clock();


void buzz_forever() {
    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS_4;   // Set PA4 HIGH
        for (volatile int i = 0; i < 10000; i++);
        GPIOA->BSRR = GPIO_BSRR_BR_4;   // Set PA4 LOW
        for (volatile int i = 0; i < 10000; i++);
    }
}

int main(void) {
    internal_clock();
    display_main();
    //init_buttons();
    //game_loop();
    
    /*enable_ports();
    init_i2c();
    //i2c_start(0x39, 0, 0);
    i2c_write(0x80, 1);
    i2c_stop();*/
    //dac_init();    // Setup DAC
    init_buzzer();

    //buzz_for_sound(1000);

    //buzz_forever();
    while (1) {
        // BALL BOUNCE - play PWM beep
        buzz_with_dac(200, 500);
        delay_ms(1000);
        // BRICK BREAK - output DAC leve
    }   //get_gesture_direction();
    return 0;
}

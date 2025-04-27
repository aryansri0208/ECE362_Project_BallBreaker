#ifndef GESTURE_H
#define GESTURE_H
#include "stm32f0xx.h"
typedef enum {
    DIR_NONE,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

void init_gesture_sensor();
Direction get_gesture_direction();
void i2c_start(uint8_t targadr, uint8_t size, uint8_t dir);
void init_i2c();
void i2c_stop();
void enable_ports();
void i2c_write(uint8_t reg, uint8_t data);
#endif
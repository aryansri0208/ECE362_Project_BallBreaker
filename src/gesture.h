#ifndef GESTURE_H
#define GESTURE_H

typedef enum {
    DIR_NONE,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

void init_gesture_sensor();
Direction get_gesture_direction();

#endif
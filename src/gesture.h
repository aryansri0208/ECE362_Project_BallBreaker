#ifndef GESTURE_H
#define GESTURE_H

#include <stdint.h>

#define APDS9960_I2C_ADDR 0x39

// I2C setup and control
void enable_ports_gesture(void);
void init_i2c(void);

void i2c_start_gesture(uint8_t targadr, uint8_t size, uint8_t dir);
void i2c_stop_gesture(void);
void i2c_waitidle_gesture(void);
int  i2c_senddata_gesture(uint8_t addr, uint8_t *data, uint8_t size);
int  i2c_recvdata_gesture(uint8_t addr, uint8_t *data, uint8_t size);
void i2c_clearnack_gesture(void);
int  i2c_checknack_gesture(void);

// Gesture sensor register read/write
void gesture_write(uint16_t loc, const char* data, uint8_t len);
void gesture_read(uint16_t loc, char data[], uint8_t len);

#endif // GESTURE_H

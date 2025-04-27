#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>
void init_eeprom();
void write_high_score(int score);


void enable_ports(void);
void init_i2c(void);
void i2c_start(uint8_t targadr, uint8_t size, uint8_t dir);
void i2c_stop(void);
void i2c_waitidle(void);
int i2c_senddata(uint8_t addr, uint8_t *data, uint8_t size);
int i2c_recvdata(uint8_t addr, uint8_t *data, uint8_t size);
void i2c_clearnack(void);
int i2c_checknack(void);
void save_high_score(uint8_t score);
uint8_t read_high_score(void);

#endif

#ifndef __I2C_HANDLER_H__
#define __I2C_HANDLER_H__

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdio.h>

typedef struct {
  uint sda_pin_number;
  uint scl_pin_number;
  bool has_pullup;
} I2CConfig;

void set_i2c(const I2CConfig *i2c, size_t len);

int reg_write(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
              uint8_t *buf, const uint8_t nbytes);

int reg_read(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
             uint8_t *buf, const uint8_t nbytes);

void scan_i2c_bus(i2c_inst_t *i2c, uint timeout);
bool reserved_addr(uint8_t addr);
int check_addr(i2c_inst_t *i2c, uint8_t addr, uint8_t *rxdata, uint timeout);

void read_temp_reg(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t nbytes);
void write_temp_reg(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t nbytes);
void read_temperature_registers(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t nbytes, const char *message);
void print_ambient_temperature(i2c_inst_t *i2c, uint8_t dev_addr);
void read_temp_hyst_limit(i2c_inst_t *i2c, uint8_t dev_addr);
void write_temp_hyst_limit(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t integer_part, uint8_t decimal_part);
void read_temp_set_limit(i2c_inst_t *i2c, uint8_t dev_addr);
void write_temp_set_limit(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t integer_part, uint8_t decimal_part);
uint8_t read_config(i2c_inst_t *i2c, uint8_t dev_addr);
uint8_t write_config(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t conf);
void print_temp_table(uint8_t integer_part, uint8_t decimal_part);

#endif
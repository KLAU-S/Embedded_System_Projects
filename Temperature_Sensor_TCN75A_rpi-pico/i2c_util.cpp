#include "i2c_util.hpp"
#include <stdint.h>
#include "config.hpp"
#include "util.hpp"

void set_i2c(const I2CConfig *i2c, size_t len) {
    for (size_t i = 0; i < len; i++) {
        gpio_set_function(i2c->sda_pin_number, GPIO_FUNC_I2C);
        gpio_set_function(i2c->scl_pin_number, GPIO_FUNC_I2C);
        if (i2c->has_pullup) {
            gpio_pull_up(i2c->sda_pin_number);
            gpio_pull_up(i2c->scl_pin_number);
        }
    }
}

int reg_write(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
              uint8_t *buf, const uint8_t nbytes) {
    int num_bytes_read = 0;
    uint8_t msg[nbytes + 1];

    msg[0] = reg;
    for (int i = 0; i < nbytes; i++) {
        msg[i + 1] = buf[i];
    }

    num_bytes_read = i2c_write_blocking(i2c_inst, addr, msg, (nbytes + 1), false);
    return num_bytes_read;
}

int reg_read(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
             uint8_t *buf, const uint8_t nbytes) {
  int num_bytes_read = 0;

  if (nbytes < 1) {
    return 0;
  }

  i2c_write_blocking(i2c_inst, addr, &reg, 1, true);
  num_bytes_read = i2c_read_blocking(i2c_inst, addr, buf, nbytes, false);

  return num_bytes_read;
}

int check_addr(i2c_inst_t *i2c, uint8_t addr, uint8_t *rxdata, uint timeout) {
  int ret = i2c_read_timeout_us(i2c, addr, rxdata, 1, false, timeout);
  return ret;
}

void scan_i2c_bus(i2c_inst_t *i2c, uint timeout) {
  printf("\nI2C Bus Scan\n");
  printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

  for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
    if (addr % 16 == 0) {
      printf("%02x ", addr);
    }

    int ret;
    uint8_t rxdata;
    if (reserved_addr(addr)) {
      printf("X");
    } else {
      ret = check_addr(i2c, addr, &rxdata, timeout);
      printf(ret <= 0 ? "." : "@");
    }

    printf(addr % 16 == 15 ? "\n" : "  ");
  }
  printf("Done.\n");
}

bool reserved_addr(uint8_t addr) {
  if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78) {
    return 1;
  } else {
    return 0;
  }
}

void read_temp_reg(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t reg_addr,
                   uint8_t *buf, uint8_t nbytes) {
  reg_read(i2c, dev_addr, reg_addr, buf, nbytes);
}

void write_temp_reg(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t reg_addr,
                    uint8_t *buf, uint8_t nbytes) {
  reg_write(i2c, dev_addr, reg_addr, buf, nbytes);
}

void read_temperature_registers(i2c_inst_t *i2c, uint8_t dev_addr,
                                uint8_t reg_addr, uint8_t *buf, uint8_t nbytes,
                                const char *message) {
  read_temp_reg(i2c, dev_addr, reg_addr, buf, nbytes);
  printf("%s\n", message);
  print_temp_table(buf[0], buf[1]);
}

void print_ambient_temperature(i2c_inst_t *i2c, uint8_t dev_addr) {
  uint8_t nbytes = 2;
  uint8_t tmp[2] = {0, 0};
  clear_screen();
  read_temperature_registers(i2c, dev_addr, AMBIENT_TEMP_REG, tmp, nbytes,
                             "Ambient Temperature");
}

void read_temp_hyst_limit(i2c_inst_t *i2c, uint8_t dev_addr) {
  uint8_t nbytes = 2;
  uint8_t tmp[2] = {0, 0};
  read_temperature_registers(i2c, dev_addr, TEMP_HYST_MIN_REG, tmp, nbytes,
                       "Temperature Hyst Limit");
}

void write_temp_hyst_limit(i2c_inst_t *i2c, uint8_t dev_addr,
                           uint8_t integer_part, uint8_t decimal_part) {
  uint8_t nbytes = 2;
  uint8_t tmp[2] = {[0]=integer_part, [1]=decimal_part};
  write_temp_reg(i2c, dev_addr, TEMP_HYST_MIN_REG, tmp, nbytes);
  read_temp_hyst_limit(i2c, dev_addr);
}

void read_temp_set_limit(i2c_inst_t *i2c, uint8_t dev_addr) {
  uint8_t nbytes = 2;
  uint8_t tmp[2] = {0, 0};
  read_temperature_registers(i2c, dev_addr, TEMP_SET_MAX_REG, tmp, nbytes,
                             "Temperature Set Limit");
}

void write_temp_set_limit(i2c_inst_t *i2c, uint8_t dev_addr,
                          uint8_t integer_part, uint8_t decimal_part) {
  uint8_t nbytes = 2;
  uint8_t tmp[2] = {[0]=integer_part, [1]=decimal_part};
  write_temp_reg(i2c, dev_addr, TEMP_SET_MAX_REG, tmp, nbytes);
  read_temp_set_limit(i2c, dev_addr);
}


uint8_t read_config(i2c_inst_t *i2c, uint8_t dev_addr) {
    uint8_t nbytes = 1;
    uint8_t tmp[1] = {0};
    read_temp_reg(i2c, dev_addr, SENSOR_CONFIG_REG, tmp, nbytes);
    return tmp[0];
}

uint8_t write_config(i2c_inst_t *i2c, uint8_t dev_addr, uint8_t conf) {
    uint8_t nbytes = 1;
    uint8_t tmp[1] = {conf};
    write_temp_reg(i2c, dev_addr, SENSOR_CONFIG_REG, tmp, nbytes);
    return 0;
}

void print_temp_table(uint8_t integer_part, uint8_t decimal_part) {
    printf("%-8s--%-8s\n", "-------", "-------");
    printf("%-8s| %-8s\n", "Temp C", "Temp F");
    printf("%-8s+ %-8s\n", "-------", "-------");
    float celsius = fixedToFloat(integer_part, decimal_part);
    printf("%-8.4f| %-8.4f\n", celsius, c2f(celsius));
}
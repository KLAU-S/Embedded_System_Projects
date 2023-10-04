#ifndef __GPIO_HANDLER_H__
#define __GPIO_HANDLER_H__

#include "pico/stdlib.h"


enum CALLBACK_FUNC {
  SCAN_I2C_BUS,
  SHOW_CONFIG,
  SHOW_DEV_ID,
  SHOW_ALERT_MENU,
  SHOW_TEMP
};

enum IRQ_CTRL {
  ENABLE_IRQ,
  DISABLE_IRQ
};

// Struct for storing GPIO configuration information
typedef struct {
  uint pin_number;
  uint pin_dir;
  bool has_irq;
  bool has_pullup;
} GpioConfig;

void set_gpio(const GpioConfig *gpio, size_t len);
void enable_irq(const GpioConfig *gpio, size_t len);
void disable_irq(const GpioConfig *gpio, size_t len);
void set_irq_callback(gpio_irq_callback_t callback);

#endif
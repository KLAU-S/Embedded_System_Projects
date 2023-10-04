#include "gpio_util.hpp"

#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/regs/intctrl.h"
#include <stddef.h>

void set_gpio(const GpioConfig *gpio, size_t len) {
    for (size_t i = 0; i < len; i++) {
        gpio_init(gpio[i].pin_number);
        gpio_set_dir(gpio[i].pin_number, gpio[i].pin_dir);

        if (gpio[i].has_pullup) {
            gpio_pull_up(gpio[i].pin_number);
        }
    }
}

void enable_irq(const GpioConfig *gpio, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (gpio[i].has_irq) {
            gpio_set_irq_enabled(gpio[i].pin_number, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
            irq_set_enabled(IO_IRQ_BANK0, true);
        }
    }
}

void disable_irq(const GpioConfig *gpio, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (gpio[i].has_irq) {
            gpio_set_irq_enabled(gpio[i].pin_number, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
            irq_set_enabled(IO_IRQ_BANK0, false);
        }
    }
}
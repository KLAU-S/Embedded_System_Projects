#include <stdio.h>
#include "config.hpp"
#include "debounce.hpp"
#include "globals.hpp"
#include "gpio_util.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "i2c_util.hpp"
#include "menu_handler.hpp"

#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "util.hpp"


void gpio_callback(uint gpio, uint32_t events) {
    volatile BtnState *target_btn;
    uint32_t btn_action;
    if (gpio == BTN0) {
        target_btn = &btns[0];
        btn_action = SCAN_I2C_BUS;
    } else if (gpio == BTN1) {
        target_btn = &btns[1];
        btn_action = SHOW_CONFIG;
    } else if (gpio == BTN2) {
        target_btn = &btns[2];
        btn_action = SHOW_DEV_ID;
    } else if (gpio == BTN3) {
        target_btn = &btns[3];
        btn_action = SHOW_ALERT_MENU;
    } else if (gpio == BTN4) {
        target_btn = &btns[4];
        btn_action = SHOW_TEMP;
    } else {
        return;
    }

    if ((events & GPIO_IRQ_EDGE_RISE) && (events & GPIO_IRQ_EDGE_FALL)) {
        reset_btn_state(target_btn);
        return;
    } else if (events & GPIO_IRQ_EDGE_RISE) {
        set_rising_edge_state(target_btn);
        return;
    } else if (events & GPIO_IRQ_EDGE_FALL) {
        set_falling_edge_state(target_btn);
    }

    bool is_btn_stable = debounce(target_btn);

    if (is_btn_stable) {
        if (btn_action == SHOW_TEMP) {
            enable_read_temp = true;
            return;
        } else {
            enable_read_temp = false;
            multicore_fifo_drain();
            multicore_fifo_push_timeout_us(btn_action, 1000);
        }
    }
}
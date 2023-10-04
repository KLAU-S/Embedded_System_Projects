#include "debounce.hpp"
#include <stdio.h>
// #include "hardware/gpio.h"
// #include "hardware/timer.h"

bool is_stable(const absolute_time_t prev_time, const absolute_time_t curr_time) {
    if (curr_time < prev_time) {
        return false;
    }
    if (curr_time > prev_time) {
        return (int64_t)(curr_time - prev_time) > DEBOUNCE_DELAY;
    }
    return false;
}

bool has_changed(bool prev_state, bool curr_state) {
    bool changed = (prev_state ^ curr_state);
    return changed;
}

bool debounce(volatile BtnState* btn) {
    if (has_changed(btn->prev_state, btn->curr_state)) {
        if (is_stable(btn->prev_time, btn->curr_time)) {
            return true;
        }
    }
    return false;
}

void set_rising_edge_state(volatile BtnState *btn) {
    btn->prev_state = 0;
    btn->curr_state = 1;
    btn->prev_time = get_absolute_time();
    btn->curr_time = 0;
}

void set_falling_edge_state(volatile BtnState *btn) {
    btn->prev_state = 1;
    btn->curr_state = 0;
    btn->curr_time = get_absolute_time();
}

void reset_btn_state(volatile BtnState *btn) {
    btn->prev_state = 0;
    btn->curr_state = 0;
    btn->prev_time = 0;
    btn->curr_time = 0;
}

void update_btn_state(volatile BtnState *btn) {
    btn->prev_state = btn->curr_state;
    btn->curr_state = gpio_get(btn->but_pin);
}

void update_btn_time(volatile BtnState *btn) {
    btn->prev_time = btn->curr_time;
    btn->curr_time = get_absolute_time();
}

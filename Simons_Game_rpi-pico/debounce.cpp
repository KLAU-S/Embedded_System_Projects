#include "debounce.hpp"
#include <cstdio>


bool is_stable(const volatile absolute_time_t& prev_time,
               const volatile absolute_time_t& curr_time) {
    if (curr_time < prev_time) {
        return false;
    }
    return true;
}


// bool is_stable(const absolute_time_t& prev_time,
//                const absolute_time_t& curr_time) {
//     if (curr_time < prev_time) {
//         return false;
//     }

//     if (curr_time > prev_time) {
//         return static_cast<int64_t>(curr_time - prev_time) > DEBOUNCE_DELAY;
//     }
//     return false;
// }

bool has_changed(bool prev_state, bool curr_state) {
    return prev_state ^ curr_state;
}

bool debounce(const volatile BtnState& btn) {
    if (has_changed(btn.prev_state, btn.curr_state) && is_stable(btn.prev_time, btn.curr_time)) {
        return true;
    }
    return false;
}

void set_rising_edge_state(volatile BtnState *btn) {
    btn->prev_state = LOW;
    btn->curr_state = HIGH;
    btn->prev_time = get_absolute_time();
    btn->curr_time = {};
}

void set_falling_edge_state(volatile BtnState *btn) {
    btn->prev_state = HIGH;
    btn->curr_state = LOW;
    btn->curr_time = get_absolute_time();
}

void reset_btn_state(volatile BtnState *btn) {
    btn->prev_state = LOW;
    btn->curr_state = LOW;
    btn->prev_time = {};
    btn->curr_time = {};
}

void update_btn_state(volatile BtnState *btn) {
    btn->prev_state = btn->curr_state;
    btn->curr_state = gpio_get(btn->but_pin);
}

void update_btn_time(volatile BtnState *btn) {
    btn->prev_time = btn->curr_time;
    btn->curr_time = get_absolute_time();
}

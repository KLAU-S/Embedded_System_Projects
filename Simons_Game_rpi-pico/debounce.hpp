#pragma once

#include "pico/stdlib.h"

constexpr uint32_t DEBOUNCE_DELAY = 200000; // Debouncing delay in microseconds
constexpr uint32_t HIGH = 1;
constexpr uint32_t LOW = 0;

// Struct for storing button state information
// but_pin the number of the button pin
// prev_state the previous state of the button
// curr_state the current state of the button
struct BtnState {
  uint but_pin;
  bool prev_state;
  bool curr_state;
  absolute_time_t prev_time;
  absolute_time_t curr_time;
};

bool debounce(const volatile BtnState& btn);
bool is_stable(const volatile absolute_time_t& prev_time, const volatile absolute_time_t& curr_time);
bool has_changed(bool prev_state, bool curr_state);
void set_rising_edge_state(volatile BtnState *btn);
void set_falling_edge_state(volatile BtnState *btn);
void reset_btn_state(volatile BtnState *btn);

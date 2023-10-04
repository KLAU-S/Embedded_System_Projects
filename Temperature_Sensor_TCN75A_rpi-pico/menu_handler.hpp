#ifndef __MENUE_HANDLER_H__
#define __MENUE_HANDLER_H__
#include "pico/stdlib.h"

enum ALERT_CONFIG_RESULT_TYPES {
  NO_CHANGE = 0,
  DISABLE_ALERT = 1 << 31,
  ENABLE_ALERT = 1 << 30,
  WRITE_TEMP_HYST_LIMIT = 1 << 29,
  WRITE_TEMP_SET_LIMIT = 1 << 28,
  READ_TEMP_HYST_LIMIT = 1 << 27,
  READ_TEMP_SET_LIMIT = 1 << 26,
};

void show_landing_page();
uint32_t show_config_menu();
uint8_t show_dev_change_menu(uint8_t default_addr);
uint32_t show_alert_menu(uint8_t *buf);
void parse_config(uint8_t conf);

#endif
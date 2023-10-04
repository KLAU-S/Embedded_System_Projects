#ifndef __CORE1_H__
#define __CORE1_H__
#include "pico/stdlib.h"

void handle_request(uint32_t request);
void handle_show_config();
void handle_show_dev_id();
void handle_show_alert_menu();

void core1_entry();

#endif
#ifndef __GLOBALS_H__ // check if __GLOBALS_H__ is not defined
#define __GLOBALS_H__ // define __GLOBALS_H__ to avoid multiple inclusions of
                      // the same header file

#include "config.hpp"
#include "debounce.hpp"
#include "i2c_util.hpp"
#include "pico/stdlib.h"

// declare bool variable enable_read_temp without defining it
extern bool enable_read_temp;
// declare pointer variable i2c of type i2c_inst_t without defining it
extern i2c_inst_t *i2c;
// declare 8-bit integer variable dev_addr without defining it
extern uint8_t dev_addr;
// declare an array of type BtnState called btns without defining it
extern volatile BtnState btns[NUMBER_OF_BTNS];
#endif // end of ifndef directive
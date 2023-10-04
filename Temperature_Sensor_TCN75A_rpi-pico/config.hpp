// Check if CONFIG_H is already defined and if not, define it.
#ifndef __CONFIG_H__
#define __CONFIG_H__

// Include header files for various functions and macros used by the program.
#include "gpio_util.hpp"
#include "hardware/i2c.h"
#include "i2c_util.hpp"
#include "pico/stdlib.h"
#include <stdio.h>

#define ONBOARD_LED 25
#define LED0 6
#define LED1 7
#define BTN0 15
#define BTN1 14
#define BTN2 13
#define BTN3 12
#define BTN4 11
#define ALERT_GP 18
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define I2C_READ_TIMEOUT_MICRO_SEC 100000
#define TCN75A_DEFAULT_ADDR 0x48
#define AMBIENT_TEMP_REG 0b00
#define SENSOR_CONFIG_REG 0b01
#define TEMP_HYST_MIN_REG 0b10
#define TEMP_SET_MAX_REG 0b11
#define TCN75A_BAUDRATE (400 * 1000)
#define BLINK_LED_DELAY 500

// Define additional constants for the number of GPIO pins, number of buttons,
// and number of I2C devices used by the program.
#define NUMBER_OF_GPIOS 9
#define NUMBER_OF_BTNS 8
#define NUMBER_OF_I2C 1

// Define bit masks for various configuration settings used by the TCN75A
// temperature sensor.
#define SHUTDOWN_MASK 0b00000001
#define ALERT_MODE_MASK 0b00000010
#define ALERT_POLARITY_MASK 0b00000100
#define FAULT_QUEUE_MASK 0b00011000
#define ADC_RESOLUTION_MASK 0b01100000
#define ONE_SHOT_MASK 0b10000000

// Declare extern variables for the GPIO and I2C configurations used by the
// program.
extern const GpioConfig proj_gpio[NUMBER_OF_GPIOS];
extern const I2CConfig proj_i2c[NUMBER_OF_I2C];

// Define values and shifts for various configurations of the device
#define DISABLE_IRQ 0 // Value to disable interrupts
#define ENABLE_IRQ 1  // Value to enable interrupts

// Bit masks for various configuration settings
#define SHUTDOWN_MODE_REQ_MASK 0b00000001    // Mask for shutdown mode req
#define COMP_INT_MODE_REQ_MASK 0b00000010    // Mask for comp/int mode req
#define ALERT_POLARITY_REQ_MASK 0b00000100   // Mask for alert polarity req
#define FAULT_QUEUE_MODE_REQ_MASK 0b00011000 // Mask for fault queue mode req
#define ADC_RESOLUTION_REQ_MASK 0b01100000   // Mask for ADC resolution req
#define ONE_SHOT_MODE_REQ_MASK 0b10000000    // Mask for one-shot mode req

// Shift values for various configuration settings
#define NO_CHANGE_SHIFT (1 << 16)        // Flag for no change req
#define SHUTDOWN_MODE_SHIFT (1 << 31)    // Flag for shutdown mode req
#define COMP_INT_MODE_SHIFT (1 << 30)    // Flag for comp/int mode req
#define ALERT_POLARITY_SHIFT (1 << 29)   // Flag for alert polarity req
#define FAULT_QUEUE_MODE_SHIFT (1 << 28) // Flag for fault queue mode req
#define ADC_RESOLUTION_SHIFT (1 << 27)   // Flag for ADC resolution req
#define ONE_SHOT_MODE_SHIFT (1 << 26)    // Flag for one-shot mode req

#endif
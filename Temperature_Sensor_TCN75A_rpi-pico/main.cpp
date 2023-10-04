#include <stdio.h>

#include "config.hpp"
#include "debounce.hpp"

#include "pico/multicore.h"

extern void core1_entry();
extern void gpio_callback(uint gpio, uint32_t events);

bool enable_read_temp = false;
i2c_inst_t *i2c = i2c0;
uint8_t dev_addr = TCN75A_DEFAULT_ADDR;
volatile BtnState btns[NUMBER_OF_BTNS] = {{BTN0, false, false, 0, 0},
                                          {BTN1, false, false, 0, 0},
                                          {BTN2, false, false, 0, 0},
                                          {BTN3, false, false, 0, 0},
                                          {BTN4, false, false, 0, 0}};

int main() {
    // Initialize the standard input and output for the program.
  stdio_init_all();
  // Set up the GPIO pins used by the project.
  set_gpio(proj_gpio, NUMBER_OF_GPIOS);
  // Enable interrupts for the GPIO pins.
  enable_irq(proj_gpio, NUMBER_OF_GPIOS);
  // Set up a callback function to be called when a GPIO interrupt occurs.
  gpio_set_irq_callback(&gpio_callback);
  // Initialize the I2C communication protocol with a specific baud rate.
  i2c_init(i2c, TCN75A_BAUDRATE);
  // Set up the I2C device(s) used by the project.
  set_i2c(proj_i2c, NUMBER_OF_I2C);
  // Launch a second core to run a separate function.
  multicore_launch_core1(core1_entry);
  // Print the current temperature using the I2C communication protocol and
  // device address.
  print_ambient_temperature(i2c, dev_addr);
  // Loop indefinitely.
  while (1) {
    // If a request is received through the multicore FIFO, act on it.
    if (multicore_fifo_rvalid()) {
      // Get the request value from the FIFO.
      uint32_t request = multicore_fifo_pop_blocking();
      // If the request is to disable interrupts, do so.
      if (request == DISABLE_IRQ) {
        disable_irq(proj_gpio, NUMBER_OF_GPIOS);
      }
      // If the request is to enable interrupts, do so.
      else if (request == ENABLE_IRQ) {
        enable_irq(proj_gpio, NUMBER_OF_GPIOS);
      }
    }
    // If temperature reading is enabled, print the current temperature.
    if (enable_read_temp) {
      print_ambient_temperature(i2c, dev_addr);
    }
    // Turn on the onboard LED.
    gpio_put(ONBOARD_LED, 1);
    // Sleep for a set amount of time.
    sleep_ms(BLINK_LED_DELAY);
    // Turn off the onboard LED.
    gpio_put(ONBOARD_LED, 0);
    // Sleep for a set amount of time.
    sleep_ms(BLINK_LED_DELAY);
  }
}

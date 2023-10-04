#include "menu_handler.hpp"

#include <stdio.h>
#include "config.hpp"
#include "hardware/address_mapped.h"
#include "i2c_util.hpp"
#include "pico/time.h"
#include "util.hpp"

/**
 * Displays a landing page with options for the user to select.
 * The user can press a button to scan I2C addresses, access the config menu,
 * access the device ID menu, access the alert menu, or print the temperature.
 */
void show_landing_page() {
  clear_screen();
  printf(" _____ ____ _   _ _____ ____    _    \n");
  printf("|_   _/ ___| \\ | |___  | ___|  / \\   \n");
  printf("  | || |   |  \\| |  / /|___ \\ / _ \\  \n");
  printf("  | || |___| |\\  | / /  ___) / ___ \\ \n");
  printf("  |_| \\____|_| \\_|/_/  |____/_/   \\_\\ \n");
  printf(" ------------------------------------------ \n");
  printf("| Press button 0 to scan addresses         | \n");
  printf("| Press button 1 to access config menu     | \n");
  printf("| Press button 2 to access device ID menu  | \n");
  printf("| Press button 3 to access alert  menu     | \n");
  printf("| Press button 4 to print temperature      | \n");
  printf(" ------------------------------------------ \n");
}

/**

    @brief Displays a configuration menu for the temperature sensor and returns
   the selected configuration. The function displays a menu with several
   configuration options for the temperature sensor. The user can select an
   option by entering the corresponding number or letter on the keyboard. If the
   user selects an option that requires further configuration, such as the
   SHUTDOWN Setting or COMP/INT Select options, the function displays a sub-menu
   with additional configuration options. Once the user has made their
   selections, the function returns a 32-bit integer that encodes the selected
   configuration options using bit shifting and bitwise OR operations.
    @return A 32-bit integer that encodes the selected configuration options.
    
    0b0000_0000_0000_0000_0000_0000_0000_0000
    No change flag: 0b0000_0000_0000_0001_0000_0000_0000_0000
    Shutdown flag: 0b1000_0000_0000_0000_0000_0000_0000_0000
    COMP/INT flag: 0b0100_0000_0000_0000_0000_0000_0000_0000


    */
uint32_t show_config_menu() {
  char option;
  while (1) {
    clear_screen();
    printf("[0] SHUTDOWN Setting\n");
    printf("[1] COMP/INT Select\n");
    printf("[2] ALERT POLARITY\n");
    printf("[3] FAULT QUEUE\n");
    printf("[4] ADC RES\n");
    printf("[5] ONE-SHOT\n");
    printf("[x] QUIT\n");
    scanf(" %c", &option);

    if (option == '0') {
      while (1) {
        clear_screen();
        printf("SHUTDOWN Setting\n");
        printf("[0] Disable Shutdown\n");
        printf("[1] Enable Shutdown\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          // Disabling shutdown
          return SHUTDOWN_MODE_SHIFT | (0b00000000);

        } else if (option == '1') {
          // Enabling shutdown
          return SHUTDOWN_MODE_SHIFT | (0b00000001);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '1') {
      while (1) {
        clear_screen();
        printf("COMP/INT Select\n");
        printf("[0] Comparator mode\n");
        printf("[1] Interrupt mode\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          return COMP_INT_MODE_SHIFT | (0b00000000);
        } else if (option == '1') {
          return COMP_INT_MODE_SHIFT | (0b00000010);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '2') {
      while (1) {
        clear_screen();
        printf("Alert Polarity\n");
        printf("[0] Active low\n");
        printf("[1] Active High\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          // Active Low selected
          return ALERT_POLARITY_SHIFT | (0b00000000);
        } else if (option == '1') {
          // Active High selected
          return ALERT_POLARITY_SHIFT | (0b00000100);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '3') {
      while (1) {
        clear_screen();
        printf("Fault Queue\n");
        printf("[0] 00\n");
        printf("[1] 01\n");
        printf("[2] 10\n");
        printf("[3] 11\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          return FAULT_QUEUE_MODE_SHIFT | (0b00000000);
        } else if (option == '1') {
          return FAULT_QUEUE_MODE_SHIFT | (0b00001000);
        } else if (option == '2') {
          return FAULT_QUEUE_MODE_SHIFT | (0b00010000);
        } else if (option == '3') {
          return FAULT_QUEUE_MODE_SHIFT | (0b00011000);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '4') {
      while (1) {
        clear_screen();
        printf("ADC Resolution\n");
        printf("[0] 9 bit or 0.5C\n");
        printf("[1] 10 bit or 0.25C\n");
        printf("[2] 11 bit or 0.125C\n");
        printf("[3] 12 bit or 0.0625C\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          return ADC_RESOLUTION_SHIFT | (0b00000000);
        } else if (option == '1') {
          return ADC_RESOLUTION_SHIFT | (0b00100000);
        } else if (option == '2') {
          return ADC_RESOLUTION_SHIFT | (0b01000000);
        } else if (option == '3') {
          return ADC_RESOLUTION_SHIFT | (0b01100000);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '5') {
      while (1) {
        clear_screen();
        printf("One SHOT setting\n");
        printf("[0] Disable\n");
        printf("[1] Enable\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          return ONE_SHOT_MODE_SHIFT | (0b00000000);
        } else if (option == '1') {
          return ONE_SHOT_MODE_SHIFT | (0b10000000);
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == 'x') {
      return (1 << 16);
    }
  }
}

/**
  @brief Displays a menu for changing the device ID and returns the selected
  device ID. The function displays a menu with several device ID options. The
  user can select a device ID by entering the corresponding number on the
  keyboard. Once the user has made their selection, the function returns the
  selected device ID as a uint8_t. If the user selects 'x', the function
  returns 0.
  @param default_addr The default device ID to be pre-selected in the menu.
  @return The selected device ID, or 0 if the user chooses to return to the
  main menu.
*/
uint8_t show_dev_change_menu(uint8_t default_addr) {
  char option = '0';
  uint8_t addr = default_addr;
  clear_screen();
  while (1) {
    printf("Change Device ID\n");
    printf("[0] 0x48\n");
    printf("[1] 0x49\n");
    printf("[2] 0x4A\n");
    printf("[3] 0x4B\n");
    printf("[4] 0x4C\n");
    printf("[5] 0x4D\n");
    printf("[6] 0x4E\n");
    printf("[7] 0x4F\n");
    printf("[x] Return to main\n");
    scanf(" %c", &option);

    switch (option) {
      case 'x':
        addr = 0;
        break;
      case '0':
        addr = 0x48;
        break;
      case '1':
        addr = 0x49;
        break;
      case '2':
        addr = 0x4A;
        break;
      case '3':
        addr = 0x4B;
        break;
      case '4':
        addr = 0x4C;
        break;
      case '5':
        addr = 0x4D;
        break;
      case '6':
        addr = 0x4E;
        break;
      case '7':
        addr = 0x4F;
        break;
      default:
        clear_screen();
        continue;
    }

    clear_screen();
    return addr;
  }
}

/**
  @brief Displays a menu for configuring the temperature alert settings and
   returns the selected configuration. The function displays a menu with several
   alert configuration options. The user can select an option by entering the
   corresponding number on the keyboard. If the user selects an option that
   requires further input, such as Write Temp Hyst Limit or Write Temp Set Limit
   options, the function prompts the user to enter a value and stores the result
   in a buffer. The function returns an integer that encodes the selected alert
   configuration option. If the user selects 'x', the function returns without
   modifying the buffer and without changing any configuration.
  @param buf A pointer to a buffer to store any user input values required for
   alert configuration.
  @return An integer that encodes the selected alert configuration option.
*/
uint32_t show_alert_menu(uint8_t *buf) {
  char option;
  while (1) {
    clear_screen();
    printf("ALERT Config\n");
    printf("[0] Write Temp Hyst Limit\n");
    printf("[1] Write Temp Set Limit\n");
    printf("[2] Show Temp Hyst Limit\n");
    printf("[3] Show Temp Set Limit\n");
    printf("[x] Return to main\n");
    scanf(" %c", &option);

    if (option == '0') {
      char input[8];
      int32_t output[2];
      clear_screen();
      printf("Enter Temp Hyst Limit: ");
      get_input(input, 8);
      bool is_success = str_to_fixed_point(input, output);
      if (is_success) {
        buf[0] = output[0];
        buf[1] = output[1];
        return WRITE_TEMP_HYST_LIMIT;
      } else {
        printf("Invalid input! Returning to the previous menu\n");
        sleep_ms(2000);
      }
    } else if (option == '1') {
      char input[8];
      int32_t output[2];
      clear_screen();
      printf("Enter Temp Set Limit: ");
      get_input(input, 8);
      bool is_success = str_to_fixed_point(input, output);
      if (is_success) {
        buf[0] = output[0];
        buf[1] = output[1];
        return WRITE_TEMP_SET_LIMIT;
      } else {
        printf("Invalid input! Returning to the previous menu\n");
        sleep_ms(2000);
      }
    } else if (option == '2') {
      return READ_TEMP_HYST_LIMIT;
    } else if (option == '3') {
      return READ_TEMP_SET_LIMIT;
    } else if (option == 'x') {
      clear_screen();
      break;
    } else {
    }
  }
}

/**
  @brief Parses a 32-bit integer that encodes temperature sensor configuration
  settings and prints them to the console. The function takes a 32-bit integer
  as input that encodes temperature sensor configuration settings. It then
  parses the integer and prints the settings in a formatted table to the
  console. The table includes settings for Shutdown, Alert Mode, Alert Polarity,
  Fault Queue, ADC Resolution, and One-Shot. If a configuration setting is
  unknown, the function prints 'UNKNOWN' in the Value column of the table.
  @param conf A 32-bit integer that encodes the temperature sensor configuration
  settings.
*/
void parse_config(uint8_t conf) {
  printf("+--------------------+--------------+\n");
  printf("|       Setting      |     Value    |\n");
  printf("+--------------------+--------------+\n");
  printf("| %-18s | ", "Shutdown:");
  if ((conf & SHUTDOWN_MASK) == 0) {
    printf("%-12s |\n", "Disable");
  } else if ((conf & SHUTDOWN_MASK) == SHUTDOWN_MASK) {
    printf("%-12s |\n", "Enable");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Alert Mode:");
  if (((conf & ALERT_MODE_MASK) >> 1) == 0) {
    printf("%-12s |\n", "Comp");
  } else if (((conf & ALERT_MODE_MASK) >> 1) == 1) {
    printf("%-12s |\n", "Intr");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Alert Polarity:");
  if (((conf & ALERT_POLARITY_MASK) >> 2) == 0) {
    printf("%-12s |\n", "Low");
  } else if (((conf & ALERT_POLARITY_MASK) >> 2) == 1) {
    printf("%-12s |\n", "High");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Fault Queue:");
  if (((conf & FAULT_QUEUE_MASK) >> 3) == 0) {
    printf("%-12d |\n", 1);
  } else if (((conf & FAULT_QUEUE_MASK) >> 3) == 1) {
    printf("%-12d |\n", 2);
  } else if (((conf & FAULT_QUEUE_MASK) >> 3) == 2) {
    printf("%-12d |\n", 4);
  } else if (((conf & FAULT_QUEUE_MASK) >> 3) == 3) {
    printf("%-12d |\n", 6);
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "ADC Resolution:");
  if (((conf & ADC_RESOLUTION_MASK) >> 5) == 0) {
    printf("%-12s |\n", "0.5C");
  } else if (((conf & ADC_RESOLUTION_MASK) >> 5) == 1) {
    printf("%-12s |\n", "0.25C");
  } else if (((conf & ADC_RESOLUTION_MASK) >> 5) == 2) {
    printf("%-12s |\n", "0.125C");
  } else if (((conf & ADC_RESOLUTION_MASK) >> 5) == 3) {
    printf("%-12s |\n", "0.0625C");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "One-Shot:");
  if (((conf & ONE_SHOT_MASK) >> 7) == 0) {
    printf("%-12s |\n", "Disable");
  } else if (((conf & ONE_SHOT_MASK) >> 7) == 1) {
    printf("%-12s |\n", "Enable");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("+--------------------+--------------+\n");
}
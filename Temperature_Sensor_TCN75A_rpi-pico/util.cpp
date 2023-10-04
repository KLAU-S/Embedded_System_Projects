#include "util.hpp"
#include <stdio.h>
#include <string.h>

float fixedToFloat(uint8_t integerPart, uint8_t decimalPart) {
    uint16_t fixedPoint = ((uint16_t)integerPart << 8) | decimalPart;
    float floatValue = (float)fixedPoint / 256.0f;
    return floatValue;
}

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

float c2f(float celsius) {
    float fahrenheit = (celsius * 9 / 5) + 32;
    return fahrenheit;
}

void get_input(char *input, int max_length) {
  int i = 0;
  char c;
  while (1) {
    c = getchar();
    if (c == 13) {
      break;
    } else if (c == 8 || c == 127) {
      if (i > 0) {
        i--;
        putchar('\b');
        putchar(' ');
        putchar('\b');
      }
    } else if (c == 27) {
      // Handle escape sequences
      c = getchar();
      if (c == '[') {
        c = getchar();
        if (c == 'D') {
          // Left arrow key
          if (i > 0) {
            i--;
            putchar('\b');
          }
        } else if (c == 'C') {
          // Right arrow key
          if (i < strlen(input)) {
            i++;
            putchar('\x1b');
            putchar('\x5b');
            putchar('\x43');
          }
        }
      }
    } else {
      if (i < max_length - 1) {
        input[i] = c;
        i++;
        putchar(c);
      }
    }
  }
  input[i] = '\0';
}


bool str_to_fixed_point(char *input, int32_t *output) {
    int32_t integer_part = 0;
    int32_t decimal_part = 0;
    int32_t decimal_place = 1;
    int32_t is_numeric = 1;
    int32_t i = 0;

    // Parse integer part
    while (input[i] != '\0' && input[i] != '.') {
        if (input[i] >= '0' && input[i] <= '9') {
            integer_part = integer_part * 10 + (input[i] - '0');
        } else {
            is_numeric = 0;
            break;
        }
        i++;
    }

    // Parse decimal part
  if (input[i] == '.') {
    i++;
    while (input[i] != '\0') {
      if (input[i] >= '0' && input[i] <= '9') {
        decimal_part = decimal_part * 10 + (input[i] - '0');
        decimal_place *= 10;
      } else {
        is_numeric = 0;
        break;
      }
      i++;
    }
  } else if (input[i] == '\0') {
    decimal_part =
        0;  // Set default decimal part to 0 if no decimal point found
  } else {
    is_numeric = 0;  // Input contains invalid character
  }

  // Check if input is numeric and print result
  if (is_numeric) {
    int result = integer_part * decimal_place + decimal_part;
    if (decimal_place == 1) {
      output[0] = (int)result;
      output[1] = 0;
      return true;
    } else {
      output[0] = (int)(result / 10);
      output[1] = result % 10;
      return true;
    }
  } else {
    printf("Error: Input is not numeric\n");
    return false;
  }
}
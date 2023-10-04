#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "debounce.hpp"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"


#define ONBOARD_LED 25
#define BLINK_LED_DELAY 500
#define LED0 12
#define LED1 13
#define LED2 14
#define LED3 15
#define BTN0 16
#define BTN1 17
#define BTN2 18
#define BTN3 19
#define BTN4 20
#define PATTERN_LEN 10
#define NUMBER_OF_BTNS 5
volatile bool waiting_for_pattern = true;
int rand_pattern[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int level = 0;
int user_input_number = 0;
int game_over = false;
int user_point = 0;

volatile BtnState btns[NUMBER_OF_BTNS] = {{BTN0, false, false, 0, 0},
                                          {BTN1, false, false, 0, 0},
                                          {BTN2, false, false, 0, 0},
                                          {BTN3, false, false, 0, 0},
                                          {BTN4, false, false, 0, 0}};

void core1_entry() {
    while (1) {
        gpio_put(ONBOARD_LED, 1);
        sleep_ms(BLINK_LED_DELAY);
        gpio_put(ONBOARD_LED, 0);
        sleep_ms(BLINK_LED_DELAY);
    }
}


void gpio_callback(uint gpio, uint32_t events) {
    volatile BtnState *target_btn;

    if (gpio == BTN0) {
        target_btn = &btns[0];
    } else if (gpio == BTN1) {
        target_btn = &btns[1];
    } else if (gpio == BTN2) {
        target_btn = &btns[2];
    } else if (gpio == BTN3) {
        target_btn = &btns[3];
    } else if (gpio == BTN4) {
        target_btn = &btns[4];
    } else {
        return;
    }
    if ((events & GPIO_IRQ_EDGE_RISE) && (events & GPIO_IRQ_EDGE_FALL)) {
        reset_btn_state(target_btn);
        return;
    } else if (events & GPIO_IRQ_EDGE_RISE) {
        set_rising_edge_state(target_btn);
        return;
    } else if (events & GPIO_IRQ_EDGE_FALL) {
        set_falling_edge_state(target_btn);
    }

    bool is_btn_stable = debounce(*target_btn);

    if (is_btn_stable) {
        if (gpio == BTN4) {
            printf("Reset game\n");
            user_point = 0;
            level = 0;
            game_over = false;
            user_input_number = 0;
            for (int i = 0; i < PATTERN_LEN; i++) {
                rand_pattern[i] = -1;
            }
            waiting_for_pattern = true;
            return;
        }
        
        if (game_over) {
            return;
        }
        int user_selected_value = -1;
        switch (gpio) {
            case BTN0:
                user_selected_value = 0;
                break;
            case BTN1:
                user_selected_value = 1;
                break;
            case BTN2:
                user_selected_value = 2;
                break;
            case BTN3:
                user_selected_value = 3;
                break;
            default:
                break;
        }
        if (user_selected_value == -1) {
            return;
        }
        if (user_input_number < level) {
            printf("[%d] of [%d] rand = %d user = %d\n", user_input_number, level, rand_pattern[user_input_number], user_selected_value);

            if (rand_pattern[user_input_number] != user_selected_value) {
                printf("Game over\n");
                game_over = true;
                return;
            }

            if (user_input_number == level - 1) {
                user_input_number = 0;
                user_point++;
                printf("Score: %d\n", user_point);
                waiting_for_pattern = true;
            } else {
                waiting_for_pattern = false;
                user_input_number++;
            }
        }
    }
}

void show_pattern(const int *pattern) {
    for (int i = 0; i <= level; i++) {
        printf("\npattern[%d] = %d\n", i, pattern[i]);
        if (pattern[i] == 0) {
            gpio_put(LED0, 1);
            gpio_put(LED1, 0);
            gpio_put(LED2, 0);
            gpio_put(LED3, 0);
        } else if (pattern[i] == 1) {
            gpio_put(LED0, 0);
            gpio_put(LED1, 1);
            gpio_put(LED2, 0);
            gpio_put(LED3, 0);
        } else if (pattern[i] == 2) {
            gpio_put(LED0, 0);
            gpio_put(LED1, 0);
            gpio_put(LED2, 1);
            gpio_put(LED3, 0);
        } else if (pattern[i] == 3) {
            gpio_put(LED0, 0);
            gpio_put(LED1, 0);
            gpio_put(LED2, 0);
            gpio_put(LED3, 1);
        } else {
            gpio_put(LED0, 0);
            gpio_put(LED1, 0);
            gpio_put(LED2, 0);
            gpio_put(LED3, 0);
        }
        sleep_ms(BLINK_LED_DELAY);
        gpio_put(LED0, 0);
        gpio_put(LED1, 0);
        gpio_put(LED2, 0);
        gpio_put(LED3, 0);
        sleep_ms(BLINK_LED_DELAY);
    }
}

void generate_pattern(int *rand_pattern) {
    int pattern = rand() % 4;
    rand_pattern[level] = pattern;
}

int main() {
    stdio_init_all();
    gpio_init(ONBOARD_LED);
    gpio_set_dir(ONBOARD_LED, true);
    gpio_init(LED0);
    gpio_set_dir(LED0, true);
    gpio_init(LED1);
    gpio_set_dir(LED1, true);
    gpio_init(LED2);
    gpio_set_dir(LED2, true);
    gpio_init(LED3);
    gpio_set_dir(LED3, true);
    srand(time(NULL));

    gpio_set_irq_enabled_with_callback(BTN0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN3, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN4, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    multicore_launch_core1(core1_entry);
    int pattern = 0;
    while (1) {
        if (game_over) {
            continue;
        }

        if (waiting_for_pattern) {
            generate_pattern(rand_pattern);
            show_pattern(rand_pattern);
            level++;
            if (level >= PATTERN_LEN) {
                level = PATTERN_LEN - 1;
            }
            waiting_for_pattern = false;
        }
    }
    return 0;
}

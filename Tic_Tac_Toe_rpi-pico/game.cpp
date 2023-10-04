#include "game.hpp"
#include "pico/multicore.h"
#include "hardware/regs/intctrl.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include<unistd.h>
#include<cstdint>
#include<cstddef>
#include<cstdio>
#include<cstdlib>

void init_gpio(GpioConfig * gpio, size_t len) {
    // Loop through the length of the GpioConfig array
    for (size_t i = 0; i < len; i++) {

        // Initialize the GPIO specified by the pin number in the current GpioConfig
        // structure
        gpio_init(gpio[i].pin_number);
        // Set the direction of the GPIO specified by the pin number and direction
        // in the current GpioConfig structure
        gpio_set_dir(gpio[i].pin_number, gpio[i].pin_dir);
    }

}

bool debounce(const BtnState& btn) {
    // Check if the previous state and current state of the button have changed
    if (has_changed(btn.prev_state, btn.curr_state)) {
        // Check if the current state of the button is stable
        if (is_stable(btn.but_pin, btn.curr_state)) {
            // Return true if the state has changed and is stable
            return true;
        }
    }
    // Return false if state has not changed or is not stable
    return false;
}

bool is_stable(const uint button,
    const bool prev_state) {
    // Wait for a specific amount of time (DEBOUNCE_DELAY)
    sleep_us(DEBOUNCE_DELAY);
    // Get the current state of the button
    uint current_state = gpio_get(button);
    // Check if the previous state and current state of the button are both high
    if (prev_state == HIGH && current_state == HIGH) {

        // Optionally print a message if the button state is stable (only if the
        // preprocessor macro "VERBOSE" is defined)
        #ifdef VERBOSE
        printf("Button state is stable/n");
        #endif
        // Return true if the button state is stable
        return true;
    }
    // Return false if the button state is not stable
    return false;

}


bool has_changed(bool prev_state, bool curr_state) {
    // Check if the previous state is LOW and the current state is HIGH
    bool changed = (prev_state == LOW && curr_state == HIGH);
    // Optionally print a message if the button state has changed (only if the
    // preprocessor macro "VERBOSE" is defined)

    // Return true if the button state has changed, false otherwise
    #ifdef VERBOSE
    if (changed) {
        printf("Button state changed\n");
    }
    #endif
    return changed;

}


void update_btn_state(BtnState * btn) {
    // Check if the current state of the button is 0
    if (btn -> curr_state == 0) {
        btn -> prev_state = 0;
    }

    // If so, set the previous state of the button to 0

    // Check if the current state of the button is 1

    // If so, set the previous state of the button to 1
    else if (btn -> curr_state == 1) {
        btn -> prev_state = 1;
    }

    // Update the current state of the button by reading the button pin
    btn -> curr_state = gpio_get(btn -> but_pin);
}

void reset_board(char * current_player, uint * moves, char( * board)[COLS],
    bool * is_game_over) {
    // Print a message indicating that the board is being reset
    printf("Reset board\n");
    // Loop over the rows and columns of the game board
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = EMPTY;
        }
    }

    // Set each cell of the game board to the "EMPTY" value

    // Reset the number of moves to 0
    * moves = 0;

    // Reset the current player to "X"
    * current_player = X;

    // Reset the game over flag to false
    * is_game_over = false;

    // Call the function "print_board" with the game board as an argument
    print_board(board);

    // Call the function "print_player_turn" with the current player as an
    // argument
    print_player_turn( * current_player);
    // Call the function "multicore_fifo_push_blocking" with "EMPTY" as an
    // argument
    multicore_fifo_push_blocking(EMPTY);
}


uint get_curr_row(const uint moves) {
    // Return the current row in the game board by dividing the number of moves by
    // the number of rows
    return (uint)(moves) / ROWS;
}


uint get_next_row(const uint moves) {
    // Return the next row in the game board by dividing the number of moves plus
    // one by the number of rows
    return (uint)(moves + 1) / ROWS;
}


uint get_next_col(const uint moves) {
    // Return the next column in the game board by taking the remainder of the
    // number of moves plus one divided by the number of columns
    return (uint)(moves + 1) % COLS;
}


uint get_curr_col(const uint moves) {
    // Return the current column in the game board by taking the remainder of the
    // number of moves divided by the number of columns
    return (uint)(moves) % COLS;
}


void update_position(uint * moves) {
    // Calculate the next row and column of the board
    uint next_row = get_next_row( * moves);
    uint next_col = get_next_col( * moves);

    // Check if the next position is within the valid range of the board
    if (is_valid_pos(next_row, next_col)) {
        ( * moves) ++;
    } else {
        #ifdef VERBOSE
        printf("End of board. Starting from top./n");
        #endif
            *
            moves = 0;
    }
}


void print_curr_pos(const uint row,
    const uint col) {
    // Print a string "Row: %u Col: %u\n" with the values of "row" and "col"
    // replacing the placeholders %u
    printf("Row: %u Col: %u\n", row, col);
}


bool is_valid_pos(const uint row,
    const uint col) {
    // Return true if both row and column are greater than or equal to 0 and less
    // than ROWS and COLS respectively
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

bool is_empty_pos(uint
    const row, uint
    const col,
        const char( * board)[COLS]) {
    // return true if the cell is empty otherwise return false
    return (board[row][col] == EMPTY);
}

void update_board(const char current_player,
    const uint moves,
        char( * board)[COLS]) {
    uint row = get_curr_row(moves);
    uint col = get_curr_col(moves);
    printf("Entering player %c input into row %d col %d\n", current_player, row, col);
    board[row][col] = current_player;
}

void print_board(const char( * board)[COLS]) {
    printf(" %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
    printf("---+---+---\n)");
    printf(" %c | %c |%c\n", board[1][0], board[1][1], board[1][2]);
    printf("---+---+---\n)");
    printf(" %c | %c |%c\n", board[2][0], board[2][1], board[2][2]);
}

void print_player_turn(const char current_player) {
    printf("Player %c turn\n", current_player);
}

void update_player_led(const char current_player) {
    if (current_player == X) {
        gpio_put(LED1, HIGH);
        gpio_put(LED2, LOW);
    }
    else if (current_player == 0) {
        gpio_put(LED1, LOW);
        gpio_put(LED2, HIGH);
    }
    else {
        gpio_put(LED1, LOW);
        gpio_put(LED2, LOW);
    }
}

void flash_winner_led() {
    uint32_t winner = (uint32_t) EMPTY;
    uint led_pin = ONBOARD_LED;
    while (true) {
        if (multicore_fifo_rvalid()) {
            winner = multicore_fifo_pop_blocking();
        }
        if ((char) winner == X) {
            led_pin = LED1;
        } else if ((char) winner == 0) {
            led_pin = LED2;
        } else {
            led_pin = ONBOARD_LED;
        }
        gpio_put(led_pin, HIGH);
        sleep_ms(BLINK_LED_DELAY);
        gpio_put(led_pin, LOW);
        sleep_ms(BLINK_LED_DELAY);
    }
}

void handle_btn1(uint * moves) {
    update_position(moves);
    print_curr_pos(get_curr_row( * moves), get_curr_col( * moves));
}

void handle_btn2(char * current_player, uint * moves, char( * board)[COLS],
    bool * is_game_over) {
    uint row = get_curr_row( * moves);
    uint col = get_curr_col( * moves);

    if (!is_valid_pos(row, col)) {
        printf("Invalid selection row %d col %d", row, col);
        return;
    }
    if (!is_empty_pos(row, col, board)) {
        printf("row %d col %d is not empty.\n", row, col);
        printf("Please select another location.\n");
        return;
    }
    update_board( * current_player, * moves, board);
    print_board(board);

    if (is_win( * current_player, board)) {
        printf("Player %c wins!\n", * current_player);
        multicore_fifo_push_blocking( * current_player); * is_game_over = true;
        printf("\n Please press reset button to start the game.\n");
        printf("\n Waiting for the reset...\n");
    } else if (is_tie(board)) {
        printf("Tie game!\n");
        reset_board(current_player, moves, board, is_game_over);
    } else {
        * moves = 0;
        * current_player = get_new_player( * current_player);
        print_player_turn( * current_player);
    }
}

char get_new_player(char current_player) {
    if (current_player == X) {
        return 0;
    } else {
        return X;
    }
}

bool is_win(const char player,
    const char( * board)[COLS]) {
    for (int i = 0; i < ROWS; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }
    for (int j = 0; j < ROWS; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    return false;
}

bool is_tie(const char( * board)[COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}
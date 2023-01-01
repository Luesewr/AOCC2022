#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

int get_value(char x) {
    if (x == '=') {
        return -2;
    } else if (x == '-') {
        return -1;
    } else {
        return x - '0';
    }
}

/*
 * Day 25, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day25/input_day25.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables:
     *  line, to store the input line.
     */
    char line[40];

    PointerList *numbers = initialize_pointerlist();

    while (fgets(line, 40, file) != NULL) {
        int size = 0;
        while (line[size] != '\r' && line[size] != '\n') {
            size++;
        }

        PointerList *number = initialize_pointerlist_of_capacity(size + 1);

        for (int i = size - 1; i >= 0; i--) {
            add_int(number, get_value(line[i]));
        }

        for (int i = 0; i < number->size; i++) {
            int value = get_int(number, i);
            if (value < 0) {
                set_int(number, i, value + 5);
                if (i == number->size - 1) {
                    add_int(number, 0);
                }
                int next_value = get_int(number, i + 1);
                set_int(number, i + 1, next_value - 1);
            }
        }

        for (int i = number->size - 1; i >= 0; i--) {
            if (get_int(number, i) != 0) {
                break;
            }
            remove_int_at(number, number->size - 1);
        }

        add_pointer(numbers, number);

    }

    PointerList *total = initialize_pointerlist();

    int digit_index = 0;

    while (1) {
        int current_total = 0;
        int added_something = 0;
        for (int i = 0; i < numbers->size; i++) {
            PointerList *current_number = get_pointer(numbers, i);

            if (digit_index >= current_number->size) {
                continue;
            }

            added_something = 1;
            current_total += get_int(current_number, digit_index);
        }

        if (!added_something) {
            break;
        }

        if (digit_index >= total->size) {
            add_int(total, 0);
        }

        int current_value = get_int(total, digit_index) + current_total;

        int next_value = current_value / 5;

        set_int(total, digit_index, current_value % 5);

        if (next_value > 0) {
            add_int(total, next_value);
        }

        digit_index++;
    }

    for (int i = 0; i < total->size; i++) {
        int current_value = get_int(total, i);
        if (current_value > 2) {
            current_value -= 5;
            set_int(total, i, current_value);

            if (i + 1 >= total->size) {
                add_int(total, 0);
            }

            int next_value = get_int(total, i + 1);
            set_int(total, i + 1, next_value + 1);
        }
    }

    char result[total->size + 1];
    result[total->size] = '\0';

    int result_index = 0;

    for (int i = total->size - 1; i >= 0; i--, result_index++) {
        int current_value = get_int(total, i);
        if (current_value >= 0) {
            result[result_index] = (char) (((int) '0') + current_value);
        } else if (current_value == -1) {
            result[result_index] = '-';
        } else {
            result[result_index] = '=';
        }
    }

    printf("%s\n", result);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
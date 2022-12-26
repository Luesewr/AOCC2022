#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"

#define DECRYPTION_KEY 811589153
#define MIX_COUNT 10

typedef struct DoubleLinkedList {
    struct DoubleLinkedList *prev;
    struct DoubleLinkedList *next;
    long long int number;
} DoubleLinkedList;

long long int abs_long(long long int x) {
    return x < 0 ? -x : x;
}

/*
 * Day 20, Part 2
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

    file = fopen("../days/day20/input_day20.txt", "r");

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
    char line[10];
    int length = 0;
    DoubleLinkedList *start_element = NULL;
    PointerList *original_order = initialize_pointerlist();

    DoubleLinkedList *zero_element = NULL;

    if (fgets(line, 10, file) != NULL) {
        length++;

        start_element = malloc(sizeof(DoubleLinkedList));
        start_element->number = strtoll(line, NULL, 10) * DECRYPTION_KEY;

        if (start_element->number == 0) {
            zero_element = start_element;
        }

        add_pointer(original_order, start_element);
    }

    DoubleLinkedList *prev = start_element;

    if (prev == NULL) {
        return 1;
    }

    /*
     * Loop over the entire input.
     */
    while (fgets(line, 10, file) != NULL) {
        length++;
        DoubleLinkedList *current = malloc(sizeof(DoubleLinkedList));
        current->number = strtoll(line, NULL, 10) * DECRYPTION_KEY;
        current->prev = prev;
        prev->next = current;

        if (current->number == 0) {
            zero_element = current;
        }

        add_pointer(original_order, current);

        prev = current;
    }

    if (zero_element == NULL) {
        return 0;
    }

    start_element->prev = prev;
    prev->next = start_element;

    for (int amount = 0; amount < MIX_COUNT; amount++) {
        for (int i = 0; i < length; i++) {
            DoubleLinkedList *current = get_pointer(original_order, i);
            long long int abs_number = abs_long(current->number) % (length - 1);

            DoubleLinkedList *current_search = current;

            DoubleLinkedList *prev_element;
            DoubleLinkedList *next_element;

            if (current->number > 0) {
                for (int j = 0; j < abs_number; j++) {
                    current_search = current_search->next;
                }

                prev_element = current_search;
                next_element = current_search->next;
            } else if (current->number < 0) {
                for (int j = 0; j < abs_number; j++) {
                    current_search = current_search->prev;
                }

                prev_element = current_search->prev;
                next_element = current_search;
            }

            if (current->number != 0) {
                DoubleLinkedList *current_prev = current->prev;
                DoubleLinkedList *current_next = current->next;

                current->next = next_element;
                current->prev = prev_element;
                next_element->prev = current;
                prev_element->next = current;

                current_next->prev = current_prev;
                current_prev->next = current_next;
            }
        }
    }

    long long int total = 0;

    for (int i = 1000; i <= 3000; i += 1000) {
        long long int moves_forward = i % length;

        DoubleLinkedList *current_search = zero_element;

        for (int j = 0; j < moves_forward; j++) {
            current_search = current_search->next;
        }

        total += current_search->number;
    }

    printf("%lld\n", total);

    fclose(file);

    delete_pointerlist(original_order);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

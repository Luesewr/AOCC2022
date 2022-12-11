#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day11_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROUND_COUNT 20

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

    file = fopen("../inputs/input_day11.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  containsCount, to keep track of how many ranges one range fully overlapped another.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[55];
    PointerList *monkeys = initialize_pointerlist();

    while (fgets(line, 55, file) != NULL) {
        IntMonkey *monkey = malloc(sizeof(IntMonkey));

        /*
         * Parse starting items.
         */
        fgets(line, 55, file);
        char *ptr = line + 18;
        int itemAmount = (findEnd(ptr) + 2) / 4;

        monkey->items = initialize_pointerlist();

        for (int i = 0; i < itemAmount; i++) {
            add_int(monkey->items, strtol(ptr, &ptr, 10));
            ptr += 2;
        }

        /*
         * Parse function.
         */
        fgets(line, 55, file);
        IntFunction *function = malloc(sizeof(IntFunction));
        ptr = line + 19;

        if (ptr[0] == 'o') {
            function->type1 = VARIABLE;
            function->data1 = 0;
            ptr += 4;
        } else {
            function->type1 = NUMBER;
            function->data1 = strtol(ptr, &ptr, 10);
            ptr += 1;
        }

        function->operator = ptr[0] == '*';
        ptr += 2;

        if (ptr[0] == 'o') {
            function->type2 = VARIABLE;
            function->data2 = 0;
        } else {
            function->type2 = NUMBER;
            function->data2 = strtol(ptr, &ptr, 10);
        }

        monkey->function = function;

        /*
         * Parse test line.
         */
        fgets(line, 55, file);
        ptr = line + 21;

        monkey->divisible_by = strtol(ptr, &ptr, 10);

        /*
         * Parse the true value line.
         */
        fgets(line, 55, file);
        ptr = line + 29;

        monkey->true_index = strtol(ptr, &ptr, 10);

        /*
         * Parse the false value line.
         */
        fgets(line, 55, file);
        ptr = line + 30;

        monkey->false_index = strtol(ptr, &ptr, 10);

        fgets(line, 55, file);

        add_pointer(monkeys, monkey);
    }

    PointerList *activities = initialize_pointerlist();
    for (int i = 0; i < monkeys->size; i++) {
        add_int(activities, 0);
    }

    for (int round = 0; round < ROUND_COUNT; ++round) {
        for (int monkeyIndex = 0; monkeyIndex < monkeys->size; ++monkeyIndex) {
            IntMonkey *cur_monkey = get_pointer(monkeys, monkeyIndex);
            PointerList *cur_items = cur_monkey->items;
            set_int(activities, monkeyIndex, get_int(activities, monkeyIndex) + cur_items->size);
            for (int i = 0; i < cur_items->size; ++i) {
                int cur_value = get_int(cur_items, i);
                IntFunction *function = cur_monkey->function;
                if (function->type1 == VARIABLE) {
                    function->data1 = cur_value;
                }
                if (function->type2 == VARIABLE) {
                    function->data2 = cur_value;
                }
                cur_value = function->operator ? function->data1 * function->data2 : function->data1 + function->data2;
                cur_value /= 3;
                int next_index = cur_value % cur_monkey->divisible_by == 0 ? cur_monkey->true_index : cur_monkey->false_index;
                add_int(((IntMonkey *) get_pointer(monkeys, next_index))->items, cur_value);
                set_pointer(cur_monkey->items, i, NULL);
            }
            cur_monkey->items->size = 0;
        }
    }

    sort_ints_with_comparator(activities, &reverseIntComparator);

    int monkey_business = get_int(activities, 0) * get_int(activities, 1);

    printf("%d\n", monkey_business);

    delete_pointerlist(activities);

    for (int i = 0; i < monkeys->size; i++) {
        IntMonkey *cur_monkey = get_pointer(monkeys, i);
        free(cur_monkey->function);
        delete_pointerlist(cur_monkey->items);
    }

    delete_pointerlist(monkeys);

    fclose(file);

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
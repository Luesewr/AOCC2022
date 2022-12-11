#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day11_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROUND_COUNT 10000

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

    long long total_divider = 1;

    while (fgets(line, 55, file) != NULL) {
        LongMonkey *monkey = malloc(sizeof(LongMonkey));

        /*
         * Parse starting items.
         */
        fgets(line, 55, file);
        char *ptr = line + 18;
        int itemAmount = (findEnd(ptr) + 2) / 4;

        monkey->items = initialize_pointerlist();

        for (int i = 0; i < itemAmount; i++) {
            add_long(monkey->items, strtol(ptr, &ptr, 10));
            ptr += 2;
        }

        /*
         * Parse function.
         */
        fgets(line, 55, file);
        LongFunction *function = malloc(sizeof(LongFunction));
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

        total_divider *= monkey->divisible_by;

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
        add_long(activities, 0);
    }

    for (int round = 0; round < ROUND_COUNT; ++round) {
        for (int monkeyIndex = 0; monkeyIndex < monkeys->size; ++monkeyIndex) {
            LongMonkey *cur_monkey = get_pointer(monkeys, monkeyIndex);
            PointerList *cur_items = cur_monkey->items;
            set_long(activities, monkeyIndex, get_long(activities, monkeyIndex) + cur_items->size);
            for (int i = 0; i < cur_items->size; ++i) {
                long long old_value = get_long(cur_items, i);
                LongFunction *function = cur_monkey->function;
                if (function->type1 == VARIABLE) {
                    function->data1 = old_value;
                }
                if (function->type2 == VARIABLE) {
                    function->data2 = old_value;
                }
                long long new_value = function->operator ? function->data1 * function->data2 : function->data1 + function->data2;
                int is_divisible = new_value % cur_monkey->divisible_by == 0;
                int next_index = is_divisible ? cur_monkey->true_index : cur_monkey->false_index;
                LongMonkey *next_monkey = (LongMonkey *) get_pointer(monkeys, next_index);
                add_long(next_monkey->items, new_value % total_divider);
                set_pointer(cur_monkey->items, i, NULL);
            }
            cur_monkey->items->size = 0;
        }
    }


    sort_long_with_comparator(activities, &reverseLongComparator);

    long long monkey_business = get_long(activities, 0) * get_long(activities, 1);

    printf("%lld\n", monkey_business);

    delete_pointerlist(activities);

    for (int i = 0; i < monkeys->size; i++) {
        LongMonkey *cur_monkey = get_pointer(monkeys, i);
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
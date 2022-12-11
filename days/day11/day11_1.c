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

    PointerList *monkeys = initialize_pointerlist();

    parse_input(monkeys);

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
                long long cur_value = get_long(cur_items, i);
                LongFunction *function = cur_monkey->function;
                if (function->type1 == VARIABLE) {
                    function->data1 = cur_value;
                }
                if (function->type2 == VARIABLE) {
                    function->data2 = cur_value;
                }
                cur_value = function->operator ? function->data1 * function->data2 : function->data1 + function->data2;
                cur_value /= 3;
                int next_index = cur_value % cur_monkey->divisible_by == 0 ? cur_monkey->true_index : cur_monkey->false_index;
                add_long(((LongMonkey *) get_pointer(monkeys, next_index))->items, cur_value);
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

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
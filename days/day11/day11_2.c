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
     * Create a list of monkeys and parse the input into it as well as store the smallest common denominator.
     */
    PointerList *monkeys = initialize_pointerlist();

    long long total_divider = parse_input(monkeys);

    /*
     * Create a list that keeps track of the amount of inspections for each monkey, initialized at 0.
     */
    PointerList *inspections = initialize_pointerlist();

    for (int i = 0; i < monkeys->size; i++) {
        add_long(inspections, 0);
    }

    /*
     * Simulate ROUND_COUNT rounds.
     */
    for (int round = 0; round < ROUND_COUNT; ++round) {

        /*
         * Loop through all the monkeys to process their items.
         */
        for (int monkeyIndex = 0; monkeyIndex < monkeys->size; ++monkeyIndex) {

            /*
             * Get the current monkey, its list of items and its function.
             */
            Monkey *cur_monkey = get_pointer(monkeys, monkeyIndex);
            PointerList *cur_items = cur_monkey->items;
            Function *function = cur_monkey->function;

            /*
             * Update the current monkeys interactions by the amount of current items.
             */
            set_long(inspections, monkeyIndex, get_long(inspections, monkeyIndex) + cur_items->size);

            for (int i = 0; i < cur_items->size; i++) {

                /*
                 * Get the current item.
                 */
                long long cur_value = get_long(cur_items, i);

                /*
                 * Set the data of either parameter to the current value if the parameter was the old variable.
                 */
                if (function->type1 == VARIABLE) {
                    function->data1 = cur_value;
                }
                if (function->type2 == VARIABLE) {
                    function->data2 = cur_value;
                }

                /*
                 * Calculate the new value of the current value.
                 */
                cur_value = function->operator ? function->data1 * function->data2 : function->data1 + function->data2;

                /*
                 * Set the new value to itself modulo the smallest common denominator in order to not let the value
                 * grow into overflows while keeping the same divisor properties for all the divisors of the monkeys.
                 */
                cur_value %= total_divider;

                /*
                 * Select the monkey to which this item will be thrown.
                 */
                int next_index = cur_value % cur_monkey->divisible_by == 0 ?
                                 cur_monkey->true_index : cur_monkey->false_index;
                Monkey *next_monkey = get_pointer(monkeys, next_index);

                /*
                 * Add the new value to the monkey to which this item will be thrown and remove it from this monkey.
                 */
                add_long(next_monkey->items, cur_value);
                free(cur_items->pointers[i]);
                cur_items->pointers[i] = NULL;
            }

            cur_monkey->items->size = 0;
        }
    }

    /*
     * Sort the amount of inspections in descending order.
     */
    sort_long_with_comparator(inspections, &reverseLongComparator);

    /*
     * Multiply the 2 largest values in the inspection list to calculate the monkey business.
     */
    long long monkey_business = get_long(inspections, 0) * get_long(inspections, 1);

    /*
     * Print the monkey business and clean up all allocated memory.
     */
    printf("%lld\n", monkey_business);

    delete_pointerlist(inspections);

    for (int i = 0; i < monkeys->size; i++) {
        Monkey *cur_monkey = get_pointer(monkeys, i);
        free(cur_monkey->function);
        delete_pointerlist(cur_monkey->items);
    }

    delete_pointerlist(monkeys);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
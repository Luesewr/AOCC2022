#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day13_functions.h"

/*
 * Day 13, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    PointerList *packets = parseInput();

    int indicesSum = 0;

    /*
     * Go through all the pairs.
     */
    for (int i = 0; i < packets->size; i += 2) {

        /*
         * Get the base lists for both elements in the pair.
         */
        PointerList *cur_one = get_pointer(packets, i);
        PointerList *cur_two = get_pointer(packets, i + 1);

        if (packetComparator(cur_one, cur_two) == -1) {
            indicesSum += i / 2 + 1;
        }
    }

    printf("%d\n", indicesSum);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
#include <stdio.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day17_functions.h"

#define WIDTH 7
#define MAX_ROCK_COUNT 1000000000000

/*
 * Day 17, Part 2
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Parse the shapes.
     */

    PointerList *shapes = initialize_pointerlist();

    parse_shapes(shapes, WIDTH);

    /*
     * Calculate the height after MAX_ROCK_COUNT rocks.
     */

    calculate_and_print_height(shapes, WIDTH, MAX_ROCK_COUNT);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
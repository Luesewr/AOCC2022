#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*
 * Day 10, Part 1
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

    file = fopen("../inputs/input_day10.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  total, to keep track of the total score.
     *  cycle, to keep track of the current cycle.
     *  X, to keep track of the X value.
     */

    char line[12];
    int total = 0;
    int cycle = 0;
    int X = 1;

    /*
     * Loop over all the lines in the input.
     */
    while (fgets(line, 12, file) != NULL) {

        /*
         * Check if the line is a noop or an addx.
         */
        if (line[0] == 'n') {

            /*
             * Add 1 to the cycle in the case of a noop, add X * cycle to the total at cycle 20 + 40k.
             */
            cycle += 1;

            if (cycle % 40 == 20) {
                total += X * cycle;
            }
        } else {

            /*
             * Parse the amount.
             */
            char *ptr = line + 5;
            int amount = strtol(ptr, NULL, 10);

            /*
             * Act according to what part of the addx we have to update the total, or not.
             */
            int mod = cycle % 40;
            cycle += 2;

            if (mod == 18 || mod == 19) {
                total += X * (cycle - (mod == 19));
            }

            X += amount;
        }
    }

    /*
     * Print the total score.
     */

    printf("%d\n", total);


    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
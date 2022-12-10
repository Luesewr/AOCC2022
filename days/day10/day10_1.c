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
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  containsCount, to keep track of how many ranges one range fully overlapped another.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[12];
    int total = 0;
    int cycle = 0;
    int X = 1;

    while (fgets(line, 12, file) != NULL) {
        if (line[0] == 'n') {
            cycle += 1;
            if (cycle % 40 == 20) {
                total += X * cycle;
            }
        } else {
            char *ptr = line + 5;
            int amount = strtol(ptr, NULL, 10);
            if (cycle % 40 == 18) {
                cycle += 2;
                total += X * cycle;
                X += amount;
            } else if (cycle % 40 == 19) {
                cycle += 1;
                total += X * cycle;
                X += amount;
                cycle += 1;
            } else {
                cycle += 2;
                X += amount;
            }
        }
    }

    printf("%d\n", total);


    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
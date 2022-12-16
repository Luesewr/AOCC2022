#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Day 1, Part 1
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

    file = fopen("../days/day1/input_day1.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  maximum total, to keep track of the elve with the most candy.
     *  current total, to keep track of the total for the currently read elve.
     *  input, to store the integer that was in the line.
     *
     * Then loop over all lines.
     */

    char line[20];
    int maxTotal = 0, curTotal = 0, input;

    while (fgets(line, 20, file) != NULL) {

        /*
         * While there's not an empty line, add the integer in the line to the current total.
         * If there is an empty line (with a newline as the first character), update the maximum total if the current total is larger.
         * Then reset the total and go back to checking the input lines.
         */

        if (line[0] != '\n') {
            input = atoi(line);
            curTotal += input;
        } else {
            if (curTotal > maxTotal) {
                maxTotal = curTotal;
            }
            curTotal = 0;
        }
    }

    /*
     * Close the file reader and print the result.
     */

    fclose(file);
    printf("%d\n", maxTotal);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
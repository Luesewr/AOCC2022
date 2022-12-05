#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Day 4, Part 1
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

    file = fopen("../inputs/input_day4.txt", "r");

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

    char line[15];
    int containsCount = 0;
    int * inputs = malloc(sizeof(int) * 4);

    while (fgets(line, 15, file) != NULL) {

        /*
         * Update the pointer to be at the start of the input line.
         * Then read the number and skip a character 4 times and store it in inputs.
         */

        char *ptr = line;
        for (int i = 0; i < 4; i++) {
            inputs[i] = (int) strtol(ptr, &ptr, 10);
            ptr++;
        }

        /*
         * Add one to the counter if the range of elve 1 fits in the range of elve 2 or the other way around.
         */

        containsCount += (inputs[0] >= inputs[2] && inputs[1] <= inputs[3]) || (inputs[2] >= inputs[0] && inputs[3] <= inputs[1]);
    }

    /*
     * Free the allocated memory, close the file reader, and print the result.
     */

    free(inputs);
    fclose(file);
    printf("%d\n", containsCount);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds", time_spent);
}

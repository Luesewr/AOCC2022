#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @param x Input 1
 * @param y Input 2
 * @return the minimum of the 2 inputs.
 */
int min(int x, int y) {
    return (x < y) * x + (x >= y) * y;
}

/**
 * @param x Input 1
 * @param y Input 2
 * @return the maximum of the 2 inputs.
 */
int max(int x, int y){
    return (x > y) * x + (x <= y) * y;
}

/*
 * Day 4, Part 2
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

    file = fopen("../days/day4/input_day4.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  overlapCount, to keep track of how many ranges overlapped.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[15];
    int overlapCount = 0;
    int * inputs = malloc(sizeof(int) * 4);
    char *ptr;

    while (fgets(line, 15, file) != NULL) {

        /*
         * Update the pointer to be at the start of the input line.
         * Then read the number and skip a character 4 times and store it in inputs.
         */

        ptr = line;
        for (int i = 0; i < 4; i++) {
            inputs[i] = (int) strtol(ptr, &ptr, 10);
            ptr++;
        }

        /*
         * Add one to the overlap count if the minimum of the last integers of both elves is
         * bigger or equal to the maximum of the first integers of both elves.
         */

        overlapCount += (min(inputs[1], inputs[3]) - max(inputs[0], inputs[2])) >= 0;
    }

    /*
     * Free all allocated memory, close the file reader, and print the result.
     */

    free(inputs);
    fclose(file);
    printf("%d\n", overlapCount);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

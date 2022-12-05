#include <stdio.h>
#include <time.h>

/*
 * Day 2, Part 1
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

    file = fopen("../inputs/input_day2.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up variables to store the input line and to keep track of the total score.
     */

    char line[7];
    int totalScore = 0;

    /*
     * Loop over all the lines in the input file.
     */

    while (fgets(line, 6, file)) {

        /*
         * Map the input line to input1 and input2
         * A and X -> 0
         * B and Y -> 1
         * C and Z -> 2
         */

        int input1 = line[0] - (int) 'A', input2 = line[2] - (int) 'X';

        /*
         * Use math expression to calculate the scores from each input pair and add it to the total score.
         */

        totalScore += ((input2 - input1 + 3) % 3 * 3 + 3) % 9 + input2 + 1;
    }

    /*
     * Close the file reader and print the total score.
     */

    fclose(file);
    printf("%d\n", totalScore);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

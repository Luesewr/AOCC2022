#include <stdio.h>
#include <time.h>

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

    file = fopen("../days/day02/input_day2.txt", "r");

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

    char line[7];
    int totalScore = 0;

    while (fgets(line, 6, file)) {

        /*
         * Map the input line to input1 and input2
         * A and X -> 0
         * B and Y -> 1
         * C and Z -> 2
         */

        int input1 = line[0] - (int) 'A', input2 = line[2] - (int) 'X';

        /*
         * Calculate the score for the match and add it to the total.
         */

        int a = (input1 + input2) % 3;
        totalScore += ((a == 0) * 3 + a) + (input2 * 3);
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

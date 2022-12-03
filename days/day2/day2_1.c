#include <stdio.h>

/*
 * Day 2, Part 1
 */
int main() {
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
     * Set up variables to keep track of the total score and to store the input line.
     */

    int totalScore = 0;

    char line[7];

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
}

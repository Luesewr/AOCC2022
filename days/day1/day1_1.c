#include <stdio.h>
#include <stdlib.h>

/*
 * Day 1, Part 1
 */
int main() {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../inputs/input_day1.txt", "r");

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

    int maxTotal = 0, curTotal = 0, input;
    char line[20];
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
}
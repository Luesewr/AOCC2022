#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include <time.h>

/**
 * A comparator used to order a list in descending order.
 * @param i input 1
 * @param j input 2
 * @return < 0 if j < i, 0 if j == i, < 0 if j > i
 */

int reverseComparator(int i, int j) {
    return j - i;
}

/*
 * Day 1, Part 2
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

    file = fopen("../inputs/input_day1.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create a new list to store all the total amounts of candy for each elve.
     */

    PointerList *list = initialize_pointerlist();

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  current total, to keep track of the total for the currently read elve.
     *  input, to store the integer that was in the line.
     *
     * Then loop over all lines.
     */

    char line[20];
    int curTotal = 0, input;

    while (fgets(line, 20, file) != NULL) {

        /*
        * While there's not an empty line, add the integer in the line to the current total.
        * If there is an empty line (with a newline as the first character), add the current total of the elve to the list of totals.
        * Then reset the total and go back to checking the input lines.
        */

        if (line[0] != '\n') {
            input = atoi(line);
            curTotal += input;
        } else {
            add_int(list, curTotal);
            curTotal = 0;
        }
    }

    /*
     * Sort the list of total amounts of candy per elve in descending order.
     */

    sort_ints_with_comparator(list, &reverseComparator);

    /*
     * Sum the first 3 elements of the descending sorted list of total amounts of candy per elve (The 3 largest elements).
     */

    int topThreeTotal = 0;
    for (int i = 0; i < 3; i++) {
        topThreeTotal += get_int(list, i);
    }

    /*
     * Close the file reader, delete the list, and print the result.
     */

    fclose(file);
    delete_pointerlist(list);
    printf("%d\n", topThreeTotal);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
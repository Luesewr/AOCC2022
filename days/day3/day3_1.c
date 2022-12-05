#include <stdio.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include <time.h>

#define MAX_ENTRY_SIZE 50

/**
 * Loops backwards in a string to find the size of the string before it reaches a newline character.
 * @param array The character array (string) with a newline character.
 * @return the size of the string before it reached a newline character.
 */
int findEnd(const char *array) {
    int i = MAX_ENTRY_SIZE;
    do {
        i--;
    } while (array[i] != '\n');
    return i;
}

/**
 * Takes an int, which is actually a secret character, and maps its value to the right priority.
 * @param x The input number (secretly a character).
 * @return The priority of the value.
 */
int mapToValue(int x) {
    return x - ((x > 92) * 96) - ((x < 92) * 38);
}

/*
 * Day 3, Part 1
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

    file = fopen("../inputs/input_day3.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variable totalScore to keep track of the total score.
     *
     * Then loop over all lines.
     */

    char line[MAX_ENTRY_SIZE];
    int totalScore = 0;

    while (fgets(line, MAX_ENTRY_SIZE, file)) {

        /*
         * Store the length and half length of the input line.
         */

        int length = findEnd(line);
        int halfLength = length / 2;

        /*
         * Initialize 2 lists to store each half of the input line and then store the priority values of those halfs.
         */

        PointerList *list1 = initialize_pointerlist_of_capacity(halfLength),
                *list2 = initialize_pointerlist_of_capacity(halfLength);

        for (int i = 0; i < halfLength; i++) {
            add_int(list1, mapToValue((int) line[i]));
        }

        for (int i = halfLength; i < length; i++) {
            add_int(list2, mapToValue((int) line[i]));
        }

        /*
         * Sort both lists based on their priorities.
         */

        sort_ints(list1);
        sort_ints(list2);

        /*
         * Check for each entry in list1 in order of priority if list2 contains it,
         * if it does set value to that value and break.
         */

        for (int i = 0;; i++) {
            int current = get_int(list1, i);
            if (contains_int(list2, current)) {
                totalScore += current;
                break;
            }
        }

        /*
         * Delete the lists and clear the input line variable for as much as it was used.
         */

        delete_pointerlist(list1);
        delete_pointerlist(list2);

        for (int i = 0; i <= length; i++) {
            line[i] = 0;
        }
    }

    /*
     * Close the file reader and print the result.
     */

    fclose(file);
    printf("%d\n", totalScore);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds", time_spent);
}
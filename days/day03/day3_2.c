#include <stdio.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include <time.h>

#define MAX_ENTRY_SIZE 55

/**
 * Loops in a string to find the size of the string before it reaches a newline character.
 * @param array The character array (string) with a newline character.
 * @return the size of the string before it reached a newline character.
 */
int findEnd(const char * array) {
    int i = 0;
    do {
        i++;
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

/**
 * Gets the index of an integer in a sorted list that will be searched multiple times.
 * @param pointerList Sorted integer list of priorities.
 * @param value The value that we're looking for.
 * @param offset The index of the last number we know for sure can't be the index.
 * @return The index of the value in the list pointerList.
 */
int index_of_int_with_cutoff(PointerList *pointerList, int value, int * offset) {

    /*
     * Look through the entries of the list from index offset to the end of the list.
     * Offset is the index of the last entry in the list that we know for sure is smaller than our current value
     * based on previous searches of the list.
     */

    for (int i = *offset; i < pointerList->size; i++) {

        /*
         * Get the entry and compare that entry to the value that we're looking for.
         */

        int current = get_int(pointerList, i);
        int result = current - value;

        /*
         * Act based upon the value of result.
         * result < 0, continue searching, the numbers are smaller than the value we're looking for.
         * result = 0, you've found the value, store the current index in offset in case this list has to be searched again.
         * result > 0, we've passed the value, it is not in this list, if our current index is greater than the current offset + 1,
         *             store that index in the offset and return -1.
         */

        if (!result) {
            *offset = i;
            return i;
        } else if (result > 0) {
            if (i > *offset + 1) {
                *offset = i - 1;
            }
            return -1;
        }
    }

    /*
     * If the entire list was searched and the value wasn't found, it's not in there so return -1.
     */

    return -1;
}

/**
 * Checks if the list contains the value value by check if the index of it returns -1.
 * @param pointerList Sorted integer list of priorities.
 * @param value The value that we are looking for.
 * @param offset The index of the last index we know is smaller than the value we're looking for.
 * @return 1 if the value is in the pointerList, 0 if not.
 */
int contains_int_with_cutoff(PointerList *pointerList, int value, int * offset) {
    return index_of_int_with_cutoff(pointerList, value, offset) != -1;
}

/*
 * Day 3, Part 2
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

    file = fopen("../days/day03/input_day3.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  totalScore, to keep track of the total score.
     *  listList, a list of lists to store all 3 lists of priority values of 3 input lines.
     *  j, to keep track if we're on a line number that is divisible by 3.
     *
     * Then loop over all lines.
     */

    char line[MAX_ENTRY_SIZE + 3];
    int totalScore = 0;
    PointerList * listList = initialize_pointerlist_of_capacity(3);
    int j = 0;

    while (fgets(line, MAX_ENTRY_SIZE, file)) {

        /*
         * Store the length of the input line.
         */

        int length = findEnd(line);

        /*
         * Create list and store all the priority values of the current line.
         */

        PointerList * list = initialize_pointerlist_of_capacity(length);
        for (int i = 0; i < length; i++) {
            add_int(list, mapToValue((int) line[i]));
        }

        /*
         * Sort the list on the priorities and add that list to the listList,
         * so it can be used in other while loop iterations.
         */

        sort_ints(list);
        add_pointer(listList, list);

        /*
         * Clear the input line variable for all that it was used for the next input line.
         */

        for (int i = 0; i < MAX_ENTRY_SIZE; i++) {
            line[i] = 0;
        }

        /*
         * Add one to the line counter and go into to if statement if we're on a line divisible by 3.
         */

        j++;

        if (j % 3 == 0) {

            /*
             * Get the first list in listList to compare the rest to and
             * initialize the offset variables to use the later to speed up the searching process.
             */

            PointerList * firstList = ((PointerList *) get_pointer(listList, 0));
            int offset2 = 0, offset3 = 0;

            /*
             * Loop through all the entries in the first list of listList
             */

            for (int i = 0, len = firstList->size; i < len; i++) {

                /*
                 * Check if the second and third list of listList also contain the current entry, if they do,
                 * add it to the total score and break.
                 */

                int current = get_int(firstList, i);
                int secondContains = contains_int_with_cutoff((PointerList *) get_pointer(listList, 1), current, &offset2);
                int thirdContains = contains_int_with_cutoff((PointerList *) get_pointer(listList, 2), current, &offset3);

                if (secondContains && thirdContains) {
                    totalScore += current;
                    break;
                }
            }

            /*
             * Delete all the lists in listList and reset the size of listList.
             */

            for (int i = 0; i < 3; i++) {
                delete_pointerlist(get_pointer(listList, i));
                listList->pointers[i] = NULL;
            }

            listList->size = 0;
        }
    }

    /*
     * Close the file reader and print the total score.
     */
    delete_pointerlist(listList);

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
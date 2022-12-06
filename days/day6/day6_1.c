#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

/*
 * Day 6, Part 1
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

    file = fopen("../inputs/input_day6.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Get the size of the input file and allocate space to input the whole file at once.
     */

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(sizeof(char) * filesize + 1);

    /*
     * Input the entire file at once into the input variable and initialize a list to use as a queue.
     */

    fgets(input, (int) filesize + 1, file);

    PointerList *list = initialize_pointerlist_of_capacity(4 + 1);

    int result = -1;

    /*
     * Loop through the characters first 4 characters in the input and add them to the list,
     * because it is impossible to satisfy the condition.
     */

    for (int i = 0; i < 4; i++) {
        add_int(list, input[i]);
    }

    /*
     * Loop through the rest of the input.
     */

    for (int i = 4; i < filesize - 1; i++) {

        /*
         * Remove the first element of list and add the new character to the end of the list.
         */

        remove_int_at(list, 0);
        add_int(list, input[i]);

        /*
         * Extract all the unique values of the list.
         */

        PointerList *uniques = unique_ints(list);

        /*
         * If there are 4 unique characters in the list, set the result index, delete the uniques list and break the loop.
         */

        if (uniques->size == 4) {
            result = i + 1;
            delete_pointerlist(uniques);
            break;
        }

        /*
         * Clean up the uniques list.
         */

        delete_pointerlist(uniques);
    }

    /*
     * Print the result and clear up allocated memory.
     */

    printf("%d\n", result);

    fclose(file);
    free(input);
    delete_pointerlist(list);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);

}
#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include <time.h>

#define STACK_COUNT 9

/*
 * Day 5, Part 2
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

    file = fopen("../inputs/input_day5.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create a 2 dimensional list, with size (STACK_COUNT, ) to mimic the stacks.
     */

    PointerList *listList = initialize_pointerlist_of_capacity(STACK_COUNT);

    for (int i = 0; i < STACK_COUNT; i++) {
        add_pointer(listList, initialize_pointerlist());
    }

    /*
     * Initialize the line variable to store the input lines.
     */

    char line[40];

    /*
     * Loop through the first part of the input, the current state of the stacks.
     */

    while (1) {

        /*
         * Get in input line.
         */

        fgets(line, 40, file);

        /*
         * Check if we're at the end of the stack parsing, if so, break.
         */

        if (line[1] <= '9' && line[1] >= '0') {
            break;
        }

        for (int i = 0; i < STACK_COUNT; i++) {

            /*
             * Get the character at the right indices in the input line and add it to its corresponding stack,
             * if it's not a space or any other trivial ASCII character.
             */

            char curChar = line[i * 4 + 1];

            if (curChar > '!') {
                add_int(get_pointer(listList, i), (int) curChar);
            }

        }

    }

    /*
     * Reverse all the stacks, because they can only be parsed backwards without restarting the file scanner.
     */

    for (int i = 0; i < STACK_COUNT; i++) {

        PointerList *current_list = get_pointer(listList, i);

        for (int j = 0; j < current_list->size / 2; j++) {
            void *temp = current_list->pointers[j];
            current_list->pointers[j] = current_list->pointers[current_list->size - 1 - j];
            current_list->pointers[current_list->size - 1 - j] = temp;
        }

    }

    /*
     * Skip the empty line.
     */

    fgets(line, 40, file);

    /*
     * Loop over the rest of the input.
     */

    while (fgets(line, 40, file) != NULL) {

        /*
         * Parse each line by skipping over the words with custom pointer updates in combination with the strtol function,
         * which also updates the pointer for the length of the integer being parsed as well as return that integer.
         */

        char *ptr = line + 5;
        int n = (int) strtol(ptr, &ptr, 10);
        ptr += 6;
        int sourceIndex = (int) strtol(ptr, &ptr, 10) - 1;
        ptr += 4;
        int destinationIndex = (int) strtol(ptr, &ptr, 10) - 1;

        /*
         * Retrieve the correct source and destination lists for the move operation.
         */

        PointerList *source = get_pointer(listList, sourceIndex);
        PointerList *destination = get_pointer(listList, destinationIndex);

        /*
         * Move the last element of the source list to the destination list n times, but moving it into the temporary
         * array elements to preserve the order of the elements.
         */

        int *elements = malloc(sizeof(int) * n + 1);

        for (int i = 0; i < n; i++) {
            int element = remove_last_int(source);
            elements[i] = element;
        }

        for (int i = n - 1; i >= 0; i--) {
            add_int(destination, elements[i]);
        }

        /*
         * Free the allocated memory for elements.
         */

        free(elements);
    }

    /*
     * Create the result string by getting the last character of all the lists in listList and appending them,
     * as well as deleting the stack lists in the process.
     */

    char *result = malloc(sizeof(char) * (STACK_COUNT + 1) + 1);

    for (int i = 0; i < STACK_COUNT; i++) {

        PointerList *current_list = get_pointer(listList, i);
        result[i] = (char) get_last_int(current_list);
        delete_pointerlist(current_list);

    }

    /*
     * Add null character to terminate the string.
     */

    result[STACK_COUNT] = '\0';

    /*
     * Print the result.
     */

    printf("%s\n", result);

    /*
     * Clean up remaining allocated space and close the file reader.
     */

    free(listList->pointers);
    free(listList);
    free(result);
    fclose(file);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds", time_spent);

}

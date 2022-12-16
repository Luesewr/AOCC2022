#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

/*
 * Day 6, Part 2
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

    file = fopen("../days/day06/input_day6.txt", "r");

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

    PointerList *list = initialize_pointerlist_of_capacity(14);

    int result = -1;

    int uniqueCount = 0;

    /*
     * Loop through the characters first 4 characters in the input and add them to the list,
     * because it is impossible to satisfy the condition.
     */

    /*
     * Loop through the characters first 4 characters in the input and add them to the list,
     * because it is impossible to satisfy the condition.
     */

    for (int i = 0; i < 14; i++) {

        if (!contains_int(list, input[i])) {
            uniqueCount++;
        }

        add_int(list, input[i]);

    }

    /*
     * Loop through the rest of the input.
     */

    for (int i = 14; i < filesize - 1; i++) {

        /*
         * Remove the first element of list and add the new character to the end of the list
         * and update the unique elements counter.
         */

        int removed = remove_int_at(list, 0);

        uniqueCount += !contains_int(list, input[i]) - !contains_int(list, removed);

        add_int(list, input[i]);

        if (uniqueCount == 14) {
            result = i + 1;
            break;
        }
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
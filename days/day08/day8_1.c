#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"

#define MAX_LINE_LENGTH 99

/**
 * Get a character in an array in a list.
 * @param list  List containing character arrays.
 * @param i     Index of array in list.
 * @param j     Index of element in array in list.
 * @return      The element at indices i and j in the 2d array/list.
 */
char get2dChar(PointerList *list, int i, int j) {
    char *string = get_pointer(list, i);
    return string[j];
}

/**
 * Set an int in an array in a list.
 * @param list  List containing integer arrays.
 * @param i     Index of the array in the list.
 * @param j     Index of the element in the array in the list.
 * @param x     Value to set the index to.
 */
void set2dInt(PointerList *list, int i, int j, int x) {
    int *visible_line = get_pointer(list, i);
    visible_line[j] = x;
}

/*
 * Day 8, Part 1
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

    file = fopen("../days/day08/input_day8.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Initialize lists to store all the data as well as their visibilities in 2 2d grids.
     */
    PointerList *stringList = initialize_pointerlist();
    PointerList *visible = initialize_pointerlist();

    /*
     * Loop over all the lines in the input file.
     */
    while (1) {

        /*
         * Read the input line into a character array and store it in string_list
         */
        char *line = malloc((MAX_LINE_LENGTH + 3) * sizeof(char));
        char *result = fgets(line, MAX_LINE_LENGTH + 3, file);

        /*
         * Break if the end of the file was reached.
         */
        if (result == NULL) {
            free(line);
            break;
        }

        /*
         * Create a visibility boolean array and set all its values to 0.
         */
        int *visible_line = malloc((MAX_LINE_LENGTH) * sizeof(int));
        for (int i = 0; i < MAX_LINE_LENGTH; ++i) {
            visible_line[i] = 0;
        }

        /*
         * Store the created arrays in their corresponding lists to create the grids.
         */
        add_pointer(visible, visible_line);
        add_pointer(stringList, line);
    }

    int line_count = stringList->size;

    /*
     * Loop over all the arrays in the lists.
     */
    for (int i = 0; i < line_count; i++) {
        /*
         * Get the right arrays in both grids and initialize two variables to keep track
         * of the maximum heights for both passovers, left and right.
         */
        char *string = get_pointer(stringList, i);
        int *visible_line = get_pointer(visible, i);
        char max_height_from_left = '-', max_height_from_right = '-';

        /*
         * Loop over all the elements in the arrays from the left and right at the same time.
         */
        for (int j = 0; j < MAX_LINE_LENGTH; j++) {
            /*
             * Check the visibility for all trees from left and right by comparing them
             * to the maximum height of the trees to the left and right of them.
             */
            int from_right_index = MAX_LINE_LENGTH - j - 1;
            char char_from_left = string[j], char_from_right = string[from_right_index];

            if (char_from_left > max_height_from_left) {
                visible_line[j] = 1;
                max_height_from_left = char_from_left;
            }

            if (char_from_right > max_height_from_right) {
                visible_line[from_right_index] = 1;
                max_height_from_right = char_from_right;
            }
        }

    }

    /*
     * Loop over all the elements in the arrays from the top and bottom at the same time.
     */
    for (int j = 1; j < MAX_LINE_LENGTH - 1; j++) {
        /*
         * Initialize two variable to keep track of the maximum heights for both passovers, top and the bottom.
         */
        char max_height_from_top = '-', max_height_from_bottom = '-';

        /*
         * Loop over all the elements at the same index of all arrays.
         */
        for (int i = 0; i < line_count; i++) {
            /*
             * Check the visibility for all trees from top and bottom by comparing them
             * to the maximum height of the trees to the top and bottom of them.
             */
            char char_from_top = get2dChar(stringList, i, j);

            if (char_from_top > max_height_from_top) {
                set2dInt(visible, i, j, 1);
                max_height_from_top = char_from_top;
            }

            int from_top_index = line_count - i - 1;
            char char_from_bottom = get2dChar(stringList, from_top_index, j);

            if (char_from_bottom > max_height_from_bottom) {
                set2dInt(visible, from_top_index, j, 1);
                max_height_from_bottom = char_from_bottom;
            }

        }
    }

    /*
     * Loop over all the elements in the visibility grid and count how many are 1.
     */
    int result = 0;
    for (int i = 0; i < line_count; i++) {
        int *visible_line = get_pointer(visible, i);
        for (int j = 0; j < MAX_LINE_LENGTH; ++j) {
            result += visible_line[j];
        }
    }

    /*
     * Print the result, cleanup the lists, and close the file reader.
     */

    printf("%d\n", result);

    delete_pointerlist(stringList);
    delete_pointerlist(visible);
    fclose(file);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

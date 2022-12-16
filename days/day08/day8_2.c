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
 * Day 8, Part 2
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
     * Initialize lists to store all the data as well as their visibility distances from all directions in 5 2d grids.
     */
    PointerList *stringList = initialize_pointerlist();
    PointerList *visible_from_left = initialize_pointerlist();
    PointerList *visible_from_right = initialize_pointerlist();
    PointerList *visible_from_top = initialize_pointerlist();
    PointerList *visible_from_bottom = initialize_pointerlist();

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
         * Create a visibility distance boolean arrays for all directions and set all their values to 0.
         */
        int *visible_line_left = malloc((MAX_LINE_LENGTH) * sizeof(int));
        int *visible_line_right = malloc((MAX_LINE_LENGTH) * sizeof(int));
        int *visible_line_top = malloc((MAX_LINE_LENGTH) * sizeof(int));
        int *visible_line_bottom = malloc((MAX_LINE_LENGTH) * sizeof(int));
        for (int i = 0; i < MAX_LINE_LENGTH; ++i) {
            visible_line_left[i] = 0;
            visible_line_right[i] = 0;
            visible_line_top[i] = 0;
            visible_line_bottom[i] = 0;
        }

        /*
         * Store the created arrays in their corresponding lists to create the grids.
         */
        add_pointer(visible_from_left, visible_line_left);
        add_pointer(visible_from_right, visible_line_right);
        add_pointer(visible_from_top, visible_line_top);
        add_pointer(visible_from_bottom, visible_line_bottom);
        add_pointer(stringList, line);
    }

    int line_count = stringList->size;

    /*
     * Loop over all the arrays in the lists in all different directions.
     * Keep track of a trailing index that stores the location of the furthest a tree can see.
     * It can be reused for the next tree if its height is greater than the last tree,
     * otherwise it is set to one below the current index.
     * Then adjust the index to point to the furthest visible tree.
     * The difference between the current index and the trailing index is then the distance,
     * store that distance in its corresponding direction list.
     */
    for (int i = 1; i < line_count - 1; i++) {
        char *string = get_pointer(stringList, i);
        int *visible_line_left = get_pointer(visible_from_left, i);
        int trailIndex = 0;

        for (int j = 1; j < MAX_LINE_LENGTH - 1; j++) {

            char char_from_left = string[j];

            if (string[j - 1] >= char_from_left) {
                trailIndex = j - 1;
            }

            while (trailIndex - 1 >= 0 && string[trailIndex] < char_from_left) {
                trailIndex--;
            }

            visible_line_left[j] = j - trailIndex;
        }

        trailIndex = MAX_LINE_LENGTH - 1;
        int *visible_line_right = get_pointer(visible_from_right, i);

        for (int j = MAX_LINE_LENGTH - 1 - 1; j >= 1; j--) {

            char char_from_right = string[j];

            if (string[j + 1] >= char_from_right) {
                trailIndex = j + 1;
            }

            while (trailIndex + 1 <= MAX_LINE_LENGTH - 1 && string[trailIndex] < char_from_right) {
                trailIndex++;
            }

            visible_line_right[j] = trailIndex - j;
        }
    }

    for (int i = 1; i < MAX_LINE_LENGTH - 1; i++) {

        int trailIndex = 0;

        for (int j = 1; j < line_count - 1; j++) {

            char char_from_top = get2dChar(stringList, j, i);

            if (get2dChar(stringList, j - 1, i) >= char_from_top) {
                trailIndex = j - 1;
            }

            while (trailIndex - 1 >= 0 && get2dChar(stringList, trailIndex, i) < char_from_top) {
                trailIndex--;
            }

            set2dInt(visible_from_top, j, i, j - trailIndex);
        }

        trailIndex = line_count - 1;

        for (int j = line_count - 1 - 1; j >= 1; j--) {

            char char_from_top = get2dChar(stringList, j, i);

            if (get2dChar(stringList, j + 1, i) >= char_from_top) {
                trailIndex = j + 1;
            }

            while (trailIndex + 1 <= line_count - 1 && get2dChar(stringList, trailIndex, i) < char_from_top) {
                trailIndex++;
            }

            set2dInt(visible_from_bottom, j, i, trailIndex - j);
        }
    }

    int max_score = 0;

    /*
     * Loop over all the direction lists at the same time, calculate their score and store the maximum score.
     */
    for (int i = 1; i < line_count - 1; i++) {

        int *visible_line_left = get_pointer(visible_from_left, i);
        int *visible_line_right = get_pointer(visible_from_right, i);
        int *visible_line_top = get_pointer(visible_from_top, i);
        int *visible_line_bottom = get_pointer(visible_from_bottom, i);

        for (int j = 1; j < MAX_LINE_LENGTH - 1; ++j) {
            int score = visible_line_left[j] * visible_line_right[j] * visible_line_top[j] * visible_line_bottom[j];
            if (score > max_score) {
                max_score = score;
            }
        }
    }

    /*
     * Print the maximum score, delete the lists, and close the file reader.
     */

    printf("%d\n", max_score);

    delete_pointerlist(stringList);
    delete_pointerlist(visible_from_left);
    delete_pointerlist(visible_from_right);
    delete_pointerlist(visible_from_top);
    delete_pointerlist(visible_from_bottom);
    fclose(file);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

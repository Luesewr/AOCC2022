#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include <time.h>
#include "day7_functions.h"

#define TOTAL_SPACE 70000000
#define SPACE_REQUIRED 30000000

/*
 * Day 7, Part 2
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Initialize variable:
     *      -directories, to store all directories in the entire structure.
     *      -files, to store all files in the entire structure.
     *      -root, the top directory.
     */

    PointerList *directories = initialize_pointerlist();
    PointerList *files = initialize_pointerlist();

    fs_element *root = create_fs_element("/\n", D, 0, NULL);

    add_pointer(directories, root);

    /*
     * Parse the input file into the structures.
     */
    parse_input(root, directories, files);

    int root_size = calculate_size(root);

    int minimum_delete_size = SPACE_REQUIRED - (TOTAL_SPACE - root_size);

    int result = root_size;

    for (int i = 0; i < directories->size; i++) {
        fs_element *current = get_pointer(directories, i);
        if (current->size - minimum_delete_size > 0 && current->size < result) {
            result = current->size;
        }
    }

    printf("%d\n", result);

    /*
     * Clean up all allocated memory as well as close the file reader.
     */
    cleanup(directories, files);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);

}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "day9_functions.h"

/*
 * Day 9, Part 2
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

    file = fopen("../inputs/input_day9.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  containsCount, to keep track of how many ranges one range fully overlapped another.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[10];

    PointerList *chain = initialize_pointerlist();
    pointSet *points = create_point_set(37);
    add_to_set(points, 0, 0);

    for (int i = 0; i < 10; i++) {
        add_pointer(chain, create_point(0, 0));
    }

    while (fgets(line, 10, file) != NULL) {
        int dx = 0, dy = 0;
        if (line[0] == 'R') {
            dx = 1;
        } else if (line[0] == 'D') {
            dy = -1;
        } else if (line[0] == 'L') {
            dx = -1;
        } else{
            dy = 1;
        }
        char *ptr = line;
        ptr += 2;
        int amount = strtol(ptr, &ptr, 10);
        for (int i = 0; i < amount; i++) {
            point *prev = get_pointer(chain, 0);
            prev->x += dx;
            prev->y += dy;
            int moved = 0;
            for (int j = 1; j < chain->size; j++) {
                point *current = get_pointer(chain, j);
                int too_far = abs(prev->x - current->x) > 1 || abs(prev->y - current->y) > 1;
                if (too_far) {
                    current->x += signum(prev->x - current->x);
                    current->y += signum(prev->y - current->y);
                }
                moved = too_far;
                prev = current;
            }
            if (moved) {
                add_to_set(points, prev->x, prev->y);
            }
        }
    }

    /*
     * Free the allocated memory, close the file reader, and print the result.
     */

    printf("%d\n", points->size);

    delete_pointerlist(chain);
    delete_point_set(points);
//    delete_pointerlist_not_pointers(uniques);
    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
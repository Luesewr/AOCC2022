#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "day9_functions.h"

#define SET_SIZE 140

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

    file = fopen("../days/day9/input_day9.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  chain, to keep track of the positions of all the knots.
     *  points, a set of points with SET_SIZE amount of buckets.
     */

    char line[10];

    PointerList *chain = initialize_pointerlist();
    pointSet *points = create_point_set(SET_SIZE);

    /*
     * Add point (0; 0) to the set as well as set the position of every knot in chain to point (0; 0).
     */
    add_to_set(points, 0, 0);

    for (int i = 0; i < 10; i++) {
        add_pointer(chain, create_point(0, 0));
    }

    /*
     * Loop over all the input lines.
     */
    while (fgets(line, 10, file) != NULL) {

        /*
         * Get and set the direction of the input.
         */
        int dx = (line[0] == 'R') - (line[0] == 'L');
        int dy = (line[0] == 'U') - (line[0] == 'D');

        /*
         * Read the amount of moves in said direction.
         */
        char *ptr = line + 2;
        int amount = strtol(ptr, &ptr, 10);

        /*
         * Repeat the amount that was in the line.
         */
        for (int i = 0; i < amount; i++) {

            /*
             * Set prev to the head of the chain.
             */
            point *prev = get_pointer(chain, 0);

            /*
             * Update the head in the direction specified in the line.
             */
            prev->x += dx;
            prev->y += dy;

            /*
             * Set moved to 0 in order to later check if the last element in the chain was moved.
             */
            int moved = 0;

            /*
             * Loop over all the locations in the chain.
             */
            for (int j = 1; j < chain->size; j++) {

                /*
                 * Get the location in the chain.
                 */
                point *current = get_pointer(chain, j);

                /*
                 * Check if the location in the chain is too far away from its predecessor.
                 */
                moved = abs(prev->x - current->x) > 1 || abs(prev->y - current->y) > 1;

                /*
                 * Update the location of this location if it's too far from its predecessor.
                 */
                if (moved) {
                    current->x += signum(prev->x - current->x);
                    current->y += signum(prev->y - current->y);
                }

                /*
                 * Update the previous element.
                 */
                prev = current;
            }

            /*
             * Add the location of the last element in the chain if it moved.
             */
            if (moved) {
                add_to_set(points, prev->x, prev->y);
            }
        }
    }

    /*
     * Print the size of the set (the result), delete the set and the chain and close the file reader.
     */
    printf("%d\n", points->size);

    delete_pointerlist(chain);
    delete_point_set(points);
    fclose(file);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
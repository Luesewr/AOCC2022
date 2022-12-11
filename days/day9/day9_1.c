#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day9_functions.h"

#define SET_SIZE 180

/*
 * Day 9, Part 1
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
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  hx and hy, to keep track of the position of the head knot.
     *  tx and ty, to keep track of the position of the tail knot.
     *  points, a set of points with SET_SIZE amount of buckets.
     */

    char line[10];
    int hx = 0, hy = 0, tx = 0, ty = 0;

    pointSet *points = create_point_set(SET_SIZE);

    /*
     * Add point (0; 0) to the set and then loop over all the input.
     */
    add_to_set(points, 0, 0);

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
        int amount = strtol(ptr, NULL, 10);

        /*
         * Repeat the amount that was in the line.
         */
        for (int i = 0; i < amount; i++) {

            /*
             * Update the head in the direction specified in the line.
             */
            hx += dx;
            hy += dy;

            /*
             * Move the tail if the head is more than 1 away from the tail in any direction.
             * Try to add the new position of the tail to the set of positions.
             */
            if (abs(hx - tx) > 1 || abs(hy - ty) > 1) {

                tx += signum(hx - tx);
                ty += signum(hy - ty);

                add_to_set(points, tx, ty);

            }
        }
    }

    /*
     * Print the size of the set (the result), delete the set and close the file reader.
     */
    printf("%d\n", points->size);

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
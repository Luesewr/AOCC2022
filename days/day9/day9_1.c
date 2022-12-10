#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day9_functions.h"

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
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  containsCount, to keep track of how many ranges one range fully overlapped another.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[10];
    int hx = 0, hy = 0, tx = 0, ty = 0;

    pointSet *points = create_point_set(37);
    add_to_set(points, 0, 0);
    int j = 0;

    while (fgets(line, 10, file) != NULL) {
//        printf("hx: %d, hy: %d, tx: %d, ty: %d\n", hx, hy, tx, ty);
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
        int amount = strtol(ptr, NULL, 10);
        for (int i = 0; i < amount; i++) {
            hx += dx;
            hy += dy;
            if (abs(hx - tx) > 1 || abs(hy - ty) > 1) {
                tx += signum(hx - tx);
                ty += signum(hy - ty);
                add_to_set(points, tx, ty);
            }
        }
        j++;
    }

//    print_set(points);

    /*
     * Free the allocated memory, close the file reader, and print the result.
     */

    printf("%d\n", points->size);

    delete_point_set(points);
    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
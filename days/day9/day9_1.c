#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"

int signum(int x) {
    return x < 0 ? -1 : (x > 0);
}

typedef struct point {
    int x;
    int y;
} point;

point *create_point(int x, int y) {
    point *new_point = malloc(sizeof(point));
    new_point->x = x;
    new_point->y = y;
    return new_point;
}

int is_point_equal(point *p1, point *p2) {
    return (p1->x == p2->x) && (p1->y == p2->y);
}

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

    PointerList *points = initialize_pointerlist();
    add_pointer(points, create_point(tx, ty));

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
        int amount = strtol(ptr, &ptr, 10);
        for (int i = 0; i < amount; i++) {
            hx += dx;
            hy += dy;
            if (abs(hx - tx) > 1 || abs(hy - ty) > 1) {
                tx += signum(hx - tx);
                ty += signum(hy - ty);
                add_pointer(points, create_point(tx, ty));
            }
        }
    }

    /*
     * Free the allocated memory, close the file reader, and print the result.
     */

    PointerList *uniques = initialize_pointerlist(points->size);
    for (int i = 0; i < points->size; i++) {
        point *current = get_pointer(points, i);
        int broken = 0;
        for (int j = 0; j < uniques->size; j++) {
            point *current_check = get_pointer(uniques, j);
            if (is_point_equal(current, current_check)) {
                broken = 1;
            }
        }
        if (!broken){
            add_pointer(uniques, current);
        }
    }

    printf("%d\n", uniques->size);

    delete_pointerlist(points);
    delete_pointerlist_not_pointers(uniques);
    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
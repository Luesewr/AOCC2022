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

void print_point(point *p) {
    printf("{x: %d, y: %d}\n", p->x, p->y);
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

    PointerList *chain = initialize_pointerlist();
    PointerList *points = initialize_pointerlist();
    add_pointer(points, create_point(0, 0));

    for (int i = 0; i < 10; i++) {
        add_pointer(chain, create_point(0, 0));
    }

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
                add_pointer(points, create_point(prev->x, prev->y));
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

    delete_pointerlist(chain);
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
#include <stdio.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day18_functions.h"

#define MAX_SIZE 25

/*
 * Day 18, Part 2
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);


    PointerList *points = initialize_pointerlist();
    PointerList *Grid3D = initialize_pointerlist_of_capacity(MAX_SIZE);

    parse_3D_grid(points, Grid3D, MAX_SIZE, -1);


    floodfill_from_origin(Grid3D, MAX_SIZE);


    int surface_area = calculate_surface_area(points, Grid3D);

    printf("%d\n", surface_area);


    delete_3D_grid(Grid3D);

    delete_pointerlist_not_pointers(Grid3D);

    delete_pointerlist(points);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define MAX_SIZE 25

typedef struct Point3D {
    int x;
    int y;
    int z;
} Point3D;

void set_3D_int(PointerList *Grid3D, int x, int y, int z, int value) {
    PointerList *Grid2D = get_pointer(Grid3D, x);
    PointerList *Grid1D = get_pointer(Grid2D, y);
    set_int(Grid1D, z, value);
}

int get_3D_int(PointerList *Grid3D, int x, int y, int z) {
    PointerList *Grid2D = get_pointer(Grid3D, x);
    PointerList *Grid1D = get_pointer(Grid2D, y);
    return get_int(Grid1D, z);
}

int count_non_neighbours(PointerList *Grid3D, Point3D *point) {
    int total = 0;

    total += get_3D_int(Grid3D, point->x + 1, point->y, point->z) == 0;
    total += get_3D_int(Grid3D, point->x - 1, point->y, point->z) == 0;
    total += get_3D_int(Grid3D, point->x, point->y + 1, point->z) == 0;
    total += get_3D_int(Grid3D, point->x, point->y - 1, point->z) == 0;
    total += get_3D_int(Grid3D, point->x, point->y, point->z + 1) == 0;
    total += get_3D_int(Grid3D, point->x, point->y, point->z - 1) == 0;

    return total;
}

int check_coords(int x, int y, int z) {
    if (x < 0 || x >= MAX_SIZE) return 0;
    if (y < 0 || y >= MAX_SIZE) return 0;
    if (z < 0 || z >= MAX_SIZE) return 0;
    return 1;
}

void flood_outside(PointerList *Grid3D, PointerList *stack, int x, int y, int z) {
    if (check_coords(x, y, z) && get_3D_int(Grid3D, x, y, z) == -1) {
        set_3D_int(Grid3D, x, y, z, 0);
        Point3D *element = malloc(sizeof(Point3D));
        Point3D values = {x, y, z};
        *element = values;
        add_pointer(stack, element);
    }
}

/*
 * Day 18, Part 2
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

    file = fopen("../days/day18/input_day18.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    char line[12];

    PointerList *Grid3D = initialize_pointerlist_of_capacity(MAX_SIZE);

    for (int x = 0; x < Grid3D->capacity; x++) {
        PointerList *Grid2D = initialize_pointerlist_of_capacity(MAX_SIZE);

        for (int y = 0; y < Grid2D->capacity; y++) {
            PointerList *Grid1D = initialize_pointerlist_of_capacity(MAX_SIZE);

            for (int z = 0; z < Grid1D->capacity; z++) {
                add_int(Grid1D, -1);
            }

            add_pointer(Grid2D, Grid1D);
        }

        add_pointer(Grid3D, Grid2D);
    }

    PointerList *points = initialize_pointerlist();

    /*
     * Loop over the input file.
     */
    while (fgets(line, 12, file) != NULL) {
        char *ptr = line;

        int x = (int) strtol(ptr, &ptr, 10) + 1;
        ptr++;
        int y = (int) strtol(ptr, &ptr, 10) + 1;
        ptr++;
        int z = (int) strtol(ptr, &ptr, 10) + 1;

        set_3D_int(Grid3D, x, y, z, 1);

        Point3D *point = malloc(sizeof(Point3D));
        Point3D point_values = {x, y, z};
        *point = point_values;

        add_pointer(points, point);
    }

    PointerList *stack = initialize_pointerlist();
    Point3D *start_element = malloc(sizeof(Point3D));
    Point3D start_values = {0, 0, 0};
    *start_element = start_values;
    add_pointer(stack, start_element);

    set_3D_int(Grid3D, 0, 0, 0, 0);

    while (stack->size > 0) {
        Point3D *cur_point = remove_at(stack, stack->size - 1);
        int x = cur_point->x;
        int y = cur_point->y;
        int z = cur_point->z;
        free(cur_point);

        flood_outside(Grid3D, stack, x + 1, y, z);
        flood_outside(Grid3D, stack, x - 1, y, z);
        flood_outside(Grid3D, stack, x, y + 1, z);
        flood_outside(Grid3D, stack, x, y - 1, z);
        flood_outside(Grid3D, stack, x, y, z + 1);
        flood_outside(Grid3D, stack, x, y, z - 1);
    }

    int surface_area = 0;

    for (int i = 0; i < points->size; i++) {
        Point3D *cur_point = get_pointer(points, i);

        int non_neighbours = count_non_neighbours(Grid3D, cur_point);

        surface_area += non_neighbours;
    }

    printf("%d\n", surface_area);

    fclose(file);

    for (int x = 0; x < Grid3D->size; x++) {
        PointerList *Grid2D = get_pointer(Grid3D, x);

        for (int y = 0; y < Grid2D->size; y++) {
            PointerList *Grid1D = get_pointer(Grid2D, y);

            delete_pointerlist(Grid1D);
        }
    }

    delete_pointerlist(Grid3D);

    delete_pointerlist(points);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
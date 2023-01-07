#include <stdio.h>
#include <stdlib.h>
#include "../../library/intList.h"
#include "day18_functions.h"

/**
 * Parse the input into a list of points and a grid of values.
 * @param points                A list to store the points in.
 * @param Grid3D                A list to store a 3D grid of the points in.
 * @param max_size              The length of the grid in all directions.
 * @param initialisation_value  The value that the points in the 3D grid will initially be set to.
 */
void parse_3D_grid(PointerList *points, PointerList *Grid3D, int max_size, int initialisation_value) {

    /*
     * Setting up the input file.
     */
    FILE *file;

    file = fopen("../days/day18/input_day18.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */
    if (file == NULL) {
        exit(1);
    }

    /*
     * Create the 3-dimensional list for storing the points of size (max_size x max_size x max_size),
     * all points are initialised as initialisation_value.
     */
    for (int x = 0; x < Grid3D->capacity; x++) {
        PointerList *Grid2D = initialize_pointerlist_of_capacity(max_size);

        for (int y = 0; y < Grid2D->capacity; y++) {
            PointerList *Grid1D = initialize_pointerlist_of_capacity(max_size);

            for (int z = 0; z < Grid1D->capacity; z++) {
                add_int(Grid1D, initialisation_value);
            }

            add_pointer(Grid2D, Grid1D);
        }

        add_pointer(Grid3D, Grid2D);
    }

    char line[12];

    /*
     * Loop over the input file and parse all the points.
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

    fclose(file);
}

/**
 * Sets a point in the grid to a value.
 * @param Grid3D    The grid of points.
 * @param x         The x coordinate of the point.
 * @param y         The y coordinate of the point.
 * @param z         The z coordinate of the point.
 * @param value     The value that the point at the coordinates will be set to.
 */
void set_3D_int(PointerList *Grid3D, int x, int y, int z, int value) {
    PointerList *Grid2D = get_pointer(Grid3D, x);
    PointerList *Grid1D = get_pointer(Grid2D, y);
    set_int(Grid1D, z, value);
}

/**
 * Get the value of a point in the grid.
 * @param Grid3D    The grid of points.
 * @param x         The x coordinate of the point.
 * @param y         The y coordinate of the point.
 * @param z         The z coordinate of the point.
 * @return          The value at the point.
 */
int get_3D_int(PointerList *Grid3D, int x, int y, int z) {
    PointerList *Grid2D = get_pointer(Grid3D, x);
    PointerList *Grid1D = get_pointer(Grid2D, y);
    return get_int(Grid1D, z);
}

/**
 * Deletes a 3D grid structure fully.
 * @param Grid3D    The grid of points.
 */
void delete_3D_grid(PointerList *Grid3D) {
    for (int x = 0; x < Grid3D->size; x++) {
        PointerList *Grid2D = get_pointer(Grid3D, x);

        for (int y = 0; y < Grid2D->size; y++) {
            PointerList *Grid1D = get_pointer(Grid2D, y);

            delete_pointerlist(Grid1D);
        }

        delete_pointerlist_not_pointers(Grid2D);
    }
}

/**
 * Count the amount of neighbouring coordinates that contain an empty space (and is not in an air pocket for part 2).
 * @param Grid3D    The grid of points.
 * @param point     The point that is being operated on.
 * @return          The number of neighbouring coordinates with no point.
 */
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

/**
 * Checks if a coordinate falls within the bounds of the grid.
 * @param x         The x coordinate of the hypothetical point.
 * @param y         The y coordinate of the hypothetical point.
 * @param z         The z coordinate of the hypothetical point.
 * @param max_size  The maximum coordinate that is allowed.
 * @return          A boolean showing if the coordinate is within the bounds of the grid.
 */
int check_coords(int x, int y, int z, int max_size) {
    if (x < 0 || x >= max_size) return 0;
    if (y < 0 || y >= max_size) return 0;
    if (z < 0 || z >= max_size) return 0;
    return 1;
}

/**
 * A sub-operation of the flood-fill algorithm that is being performed.
 * @param Grid3D    The grid of points.
 * @param stack     A list that acts like a stack for the flood-fill algorithm.
 * @param x         The x coordinate of the current point.
 * @param y         The y coordinate of the current point.
 * @param z         The z coordinate of the current point.
 * @param max_size  The maximum coordinate that is allowed.
 */
void flood_outside(PointerList *Grid3D, PointerList *stack, int x, int y, int z, int max_size) {
    if (check_coords(x, y, z, max_size) && get_3D_int(Grid3D, x, y, z) == -1) {
        set_3D_int(Grid3D, x, y, z, 0);

        Point3D *element = malloc(sizeof(Point3D));
        Point3D values = {x, y, z};
        *element = values;

        add_pointer(stack, element);
    }
}

/**
 * Performs a flood-fill algorithm from the (0, 0, 0) point.
 * @param Grid3D    The grid of points.
 * @param max_size  The maximum coordinate that is allowed.
 */
void flood_fill_from_origin(PointerList *Grid3D, int max_size) {
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

        flood_outside(Grid3D, stack, x + 1, y, z, max_size);
        flood_outside(Grid3D, stack, x - 1, y, z, max_size);
        flood_outside(Grid3D, stack, x, y + 1, z, max_size);
        flood_outside(Grid3D, stack, x, y - 1, z, max_size);
        flood_outside(Grid3D, stack, x, y, z + 1, max_size);
        flood_outside(Grid3D, stack, x, y, z - 1, max_size);
    }

    delete_pointerlist(stack);
}

/**
 * Calculates the surface area for all the points.
 * @param points    A list of points.
 * @param Grid3D    The grid of points.
 * @return
 */
int calculate_surface_area(PointerList *points, PointerList *Grid3D) {
    int surface_area = 0;

    for (int i = 0; i < points->size; i++) {
        Point3D *cur_point = get_pointer(points, i);

        int non_neighbours = count_non_neighbours(Grid3D, cur_point);

        surface_area += non_neighbours;
    }

    return surface_area;
}
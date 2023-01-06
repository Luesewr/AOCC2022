#include <stdio.h>
#include <stdlib.h>
#include "day17_functions.h"
#include "../../library/pointerList.h"
#include "../../library/intList.h"

/**
 * Parse the shapes text file into a list of shape objects.
 * @param shapes    The list to store the shapes in.
 * @param width     The width of the vertical chamber.
 */
void parse_shapes(PointerList *shapes, int width) {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day17/shapes.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        exit(1);
    }

    char shape_line[width];

    PointerList *shapes_list = initialize_pointerlist();
    PointerList *widths = initialize_pointerlist();

    add_pointer(shapes_list, initialize_pointerlist());

    /*
     * Loop over the input file.
     */

    while (fgets(shape_line, 200, file) != NULL) {

        /*
         * Get the length of the current shape.
         */

        int length = 0;

        while (shape_line[length] != '\n' && shape_line[length] != '\r') {
            length++;
        }

        /*
         * If there isn't a shape on the current line, make a new shape in the shapes list and go the next line.
         */

        if (length == 0) {
            add_pointer(shapes_list, initialize_pointerlist());
            continue;
        }

        /*
         * Store the width of the current shape.
         */

        add_int(widths, length);

        /*
         * Store the values of the shape and map the input to 1's and 0's.
         */

        int *shape_value_line = malloc(sizeof(int) * length);

        for (int i = 0; i < length; i++) {
            shape_value_line[i] = shape_line[i] < '.';
        }

        /*
         * Add the current line to the last shape in the shapes list.
         */

        PointerList *current_shape = get_pointer(shapes_list, shapes_list->size - 1);
        add_pointer(current_shape, shape_value_line);
    }

    /*
     * Now that all the lines have been processed we can store all the data in the actual shape objects.
     */

    /*
     * Loop through all the shapes.
     */

    for (int i = 0, width_index = 0; i < shapes_list->size; i++) {

        /*
         * Create a new shape.
         */

        Shape *new_shape = malloc(sizeof(Shape));

        PointerList *current_shape = get_pointer(shapes_list, i);

        /*
         * Set the metadata of the shape.
         */

        new_shape->width = get_int(widths, width_index);
        new_shape->height = current_shape->size;

        /*
         * Copy the lines into the shape.
         */

        new_shape->values = malloc(sizeof(int *) * current_shape->size);

        for (int j = current_shape->size - 1, k = 0; j >= 0; j--, width_index++, k++) {
            int *shape_value_line = get_pointer(current_shape, k);
            new_shape->values[j] = shape_value_line;
        }

        /*
         * Add the new shape to the shapes list.
         */

        add_pointer(shapes, new_shape);

        /*
         * Delete the parsed shape.
         */

        delete_pointerlist_not_pointers(current_shape);
    }

    /*
     * Delete all list that will not be used anymore.
     */

    delete_pointerlist_not_pointers(shapes_list);

    delete_pointerlist(widths);

    fclose(file);
}

/**
 * Calculates whether the shape being placed will intersect with shapes have already been placed in the cave.
 * @param shape     The shape that will be placed.
 * @param sx        The x coordinate where the bottom left of the shape will be placed.
 * @param sy        The y coordinate where the bottom left of the shape will be placed.
 * @param grid      The grid which is the cave.
 * @return
 */
int intersects(Shape *shape, int sx, int sy, PointerList *grid) {
    /*
     * Go through all values in the shape.
     */

    int **values = shape->values;

    for (int y = 0; y < shape->height; y++) {

        int *value_row = get_pointer(grid, sy + y);

        for (int x = 0; x < shape->width; x++) {

            /*
             * If the current position in the grid has a placed cell and the current position in the shape
             * also has a placed cell it intersects.
             */
            if (values[y][x] && value_row[sx + x]) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Places a shape on the grid.
 * @param shape     The shape being placed.
 * @param sx        The x coordinate of the shape being placed.
 * @param sy        The y coordinate of the shape being placed.
 * @param grid      The grid which is the cave.
 */
void place_shape(Shape *shape, int sx, int sy, PointerList *grid) {

    /*
     * Go through all values in the shape.
     */

    int **values = shape->values;

    for (int y = 0; y < shape->height; y++) {

        int *value_row = get_pointer(grid, sy + y);

        for (int x = 0; x < shape->width; x++) {

            /*
             * If the shape has a placed cell in the current position, turn on the current position in the grid.
             */
            if (values[y][x]) {
                value_row[sx + x] = 1;
            }
        }
    }
}

/**
 * Update the values in the given array to be the relative height difference between the highest points in each column.
 * @param grid              The grid which is the cave.
 * @param highest_points
 * @param width
 */
void get_highest_points(PointerList *grid, int *highest_points, int width) {
    /*
     * Initialise all values in the highest points to 0.
     */
    for (int i = 0; i < width; i++) {
        highest_points[i] = 0;
    }

    /*
     * Move down from the top of the grid to find the first cells that are filled in each column.
     */
    int points_gotten = 0;
    int y = grid->size - 1;
    while (points_gotten < width && y >= 0) {
        int *values = get_pointer(grid, y);
        for (int i = 0; i < width; i++) {
            if (highest_points[i] == 0 && values[i] == 1) {
                highest_points[i] = y;
                points_gotten++;
            }
        }
        y--;
    }

    /*
     * Map the highest points to the relative difference in heights for each column.
     */

    for (int i = 0; i < width - 1; i++) {
        highest_points[i] = highest_points[i] - highest_points[i + 1];
    }

    highest_points[width - 1] = 0;
}

/**
 * Calculate the highest point after max_rock_count rocks are placed.
 * @param shapes            A list containing all the shapes.
 * @param width             The width of the cave.
 * @param max_rock_count    The amount of rocks that will be placed.
 */
void calculate_and_print_height(PointerList *shapes, int width, long long int max_rock_count) {

    FILE *file = fopen("../days/day17/input_day17.txt", "r");

    /*
     * Get the size of the input file and allocate space to input the whole file at once.
     */

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file) + 5;
    fseek(file, 0, SEEK_SET);

    char line[filesize];

    if (fgets(line, (int) filesize, file) == NULL) {
        exit(1);
    }

    fclose(file);

    /*
     * Calculate the length of the input.
     */

    int length = 0;

    while (line[length] != '\n' && line[length] != '\r') {
        length++;
    }

    /*
     * Make a grid to store all the placed shapes.
     */

    PointerList *grid = initialize_pointerlist();

    /*
     * Keep track of the highest points and the offset that is calculated by skipping.
     */

    long long int highest_point = 0;
    long long int highest_point_offset = 0;

    /*
     * An array to store the highest points of each column.
     */

    int *highest_points = malloc(sizeof(int) * width);

    /*
     * Lists to keep track of the data at specific points in time.
     */
    PointerList *prev_highest_points = initialize_pointerlist();
    PointerList *prev_rocks_placed = initialize_pointerlist();
    PointerList *prev_highest_point = initialize_pointerlist();

    /*
     * The index of the current shape and movement.
     */
    long long int shape_index = 0;
    long long int movement_index = 0;

    /*
     * A boolean to check if we've skipped already.
     */
    int skipped = 0;

    /*
     * Simulate shapes being placed until the max_rock_count is reached.
     */
    while (shape_index < max_rock_count) {

        /*
         * Get the right shape at an offset of (2, 3) from the left and highest point.
         */
        Shape *current_shape = get_pointer(shapes, (int) (shape_index % shapes->size));

        int x = 2;
        int y = (int) highest_point + 3;

        /*
         * Expand the grid such that the new shape will fit on the grid for sure.
         */
        while (grid->size < y + current_shape->height) {
            int *empty_line = malloc(sizeof(int) * width);

            for (int i = 0; i < width; i++) {
                empty_line[i] = 0;
            }

            add_pointer(grid, empty_line);
        }

        /*
         * Find the location where the shape should be placed and place it.
         */
        while (1) {

            /*
             * Simulate the horizontal movement of the shape.
             */
            char current_movement = line[movement_index % length];
            x += ((current_movement == '>') && x + current_shape->width < width && !intersects(current_shape, x + 1, y, grid)) -
                 ((current_movement == '<') && x > 0 && !intersects(current_shape, x - 1, y, grid));

            /*
             * Go to the next movement.
             */
            movement_index++;

            /*
             * Check if relative heights the of the highest points of all the columns have been
             * in this current shape before.
             */
            if ((movement_index % length == 0) && !skipped) {

                /*
                 * Get the relative height difference between the highest points of the columns.
                 */
                get_highest_points(grid, highest_points, width);

                /*
                 * Go through the previously store relative heights.
                 */
                for (int i = 0; i < prev_highest_points->size; i++) {

                    /*
                     * If the shapes aren't the same as the one stored, skip it.
                     */
                    if ((((i + 1) * movement_index) % shapes->size) != (shape_index % shapes->size)) {
                        continue;
                    }

                    /*
                     * Check if the relative heights are the same.
                     */

                    int *cur_prev_highest_points = get_pointer(prev_highest_points, i);
                    int is_same = 1;

                    for (int j = 0; j < width; j++) {
                        if (highest_points[j] != cur_prev_highest_points[j]) {
                            is_same = 0;
                            break;
                        }
                    }

                    /*
                     * If the relative heights are the same, store the amount of loops that we can skip
                     * in the highest_points_offset and update the shape_index.
                     */

                    if (is_same) {
                        long long int rocks_placed_at_capture = get_int(prev_rocks_placed, i);
                        long long int highest_point_at_capture = get_int(prev_highest_point, i);
                        highest_point_offset =
                                ((max_rock_count - rocks_placed_at_capture) / (shape_index - rocks_placed_at_capture) -
                                 1) * (highest_point - highest_point_at_capture);
                        shape_index =
                                (max_rock_count - rocks_placed_at_capture) / (shape_index - rocks_placed_at_capture) *
                                (shape_index - rocks_placed_at_capture) + rocks_placed_at_capture;
                        skipped = 1;
                    }
                }

                /*
                 * Store the relative heights and all other relevant info in lists to be accessed later.
                 */
                if (!skipped) {
                    add_pointer(prev_highest_points, highest_points);
                    highest_points = malloc(sizeof(int) * width);
                    add_int(prev_rocks_placed, (int) shape_index);
                    add_int(prev_highest_point, (int) highest_point);
                }
            }

            /*
             * If the shape is about to overlap with another placed shape in the grid or reached the ground,
             * place the shape and break the while loop.
             */

            if (y <= 0 || intersects(current_shape, x, y - 1, grid)) {
                place_shape(current_shape, x, y, grid);
                if (highest_point < y + current_shape->height) {
                    highest_point = y + current_shape->height;
                }
                break;
            }

            /*
             * If the shape wasn't placed, move it down one and simulate it another step.
             */
            y--;
        }

        /*
         * Go to the next shape.
         */
        shape_index++;

    }

    /*
     * Print the highest point.
     */

    printf("%lld\n", highest_point + highest_point_offset);

    /*
     * Delete the allocated memory.
     */

    delete_pointerlist(grid);

    for (int i = 0; i < shapes->size; i++) {
        Shape *current_shape = get_pointer(shapes, i);

        for (int j = 0; j < current_shape->height; j++) {
            free(current_shape->values[j]);
        }

        free(current_shape->values);
    }

    delete_pointerlist(shapes);

    delete_pointerlist(prev_highest_points);

    delete_pointerlist(prev_highest_point);

    delete_pointerlist(prev_rocks_placed);

    free(highest_points);
}

//void print_grid(PointerList *grid) {
//    for (int y = grid->size - 1; y >= 0; y--) {
//        int *value_row = get_pointer(grid, y);
//        for (int x = 0; x < WIDTH; x++) {
//            printf("%d ", value_row[x]);
//        }
//        printf("\n");
//    }
//}
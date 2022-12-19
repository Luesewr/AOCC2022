#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define WIDTH 7
#define MAX_ROCK_COUNT 1000000000000

typedef struct Shape {
    int width;
    int height;
    int **values;
} Shape;

int intersects(Shape *shape, int sx, int sy, PointerList *grid) {
    int intersects = 0;

    int **values = shape->values;
    for (int y = 0; y < shape->height; y++) {
        int *value_row = get_pointer(grid, sy + y);
        for (int x = 0; x < shape->width; x++) {
            if (values[y][x] && value_row[sx + x]) {
                intersects = 1;
            }
        }
    }

    return intersects;
}

void place_shape(Shape *shape, int sx, int sy, PointerList *grid) {
    int **values = shape->values;

    for (int y = 0; y < shape->height; y++) {
        int *value_row = get_pointer(grid, sy + y);
        for (int x = 0; x < shape->width; x++) {
            if (values[y][x]) {
                value_row[sx + x] = 1;
            }
        }
    }
}

void print_grid(PointerList *grid) {
    for (int y = grid->size - 1; y >= grid->size - 30; y--) {
        int *value_row = get_pointer(grid, y);
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", (value_row[x] * ('#' - '.')) + '.');
        }
        printf("\n");
    }
}

void get_highest_points(PointerList *grid, int *highest_points) {
    for (int i = 0; i < WIDTH; i++) {
        highest_points[i] = 0;
    }
    int points_gotten = 0;
    int y = grid->size - 1;
    while (points_gotten < WIDTH && y >= 0) {
        int *values = get_pointer(grid, y);
        for (int i = 0; i < WIDTH; i++) {
            if (highest_points[i] == 0 && values[i] == 1) {
                highest_points[i] = y;
                points_gotten++;
            }
        }
        y--;
    }
}

/*
 * Day 17, Part 2
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

    file = fopen("../days/day17/shapes.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    char shape_line[WIDTH];

    PointerList *shapes_list = initialize_pointerlist();
    PointerList *widths = initialize_pointerlist();

    add_pointer(shapes_list, initialize_pointerlist());

    /*
     * Loop over the input file.
     */
    while (fgets(shape_line, 200, file) != NULL) {
        int length = 0;
        while (shape_line[length] != '\n' && shape_line[length] != '\r') {
            length++;
        }

        if (length == 0) {
            add_pointer(shapes_list, initialize_pointerlist());
            continue;
        }

        add_int(widths, length);

        int *shape_value_line = malloc(sizeof(int) * length);
        for (int i = 0; i < length; i++) {
            shape_value_line[i] = shape_line[i] < '.';
        }

        PointerList *current_shape = get_pointer(shapes_list, shapes_list->size - 1);
        add_pointer(current_shape, shape_value_line);
    }

    PointerList *shapes = initialize_pointerlist();

    for (int i = 0, width_index = 0; i < shapes_list->size; i++) {
        Shape *new_shape = malloc(sizeof(Shape));
        PointerList *current_shape = get_pointer(shapes_list, i);
        new_shape->width = get_int(widths, width_index);
        new_shape->height = current_shape->size;
        new_shape->values = malloc(sizeof(int *) * current_shape->size);

        for (int j = current_shape->size - 1, k = 0; j >= 0; j--, width_index++, k++) {
            int *shape_value_line = get_pointer(current_shape, k);
            new_shape->values[j] = shape_value_line;
        }

        add_pointer(shapes, new_shape);

        delete_pointerlist_not_pointers(current_shape);
    }

    delete_pointerlist(widths);
    delete_pointerlist_not_pointers(shapes_list);

    fclose(file);

    file = fopen("../days/day17/input_day17.txt", "r");

    /*
     * Get the size of the input file and allocate space to input the whole file at once.
     */

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char line[filesize];

    if (fgets(line, (int) filesize, file) == NULL) {
        exit(1);
    }

    int length = 0;

    while (line[length] != '\n' && line[length] != '\r') {
        length++;
    }

    PointerList *grid = initialize_pointerlist();
    long long int highest_point = 0;
    long long int highest_point_offset;
    int *highest_points = malloc(sizeof(int) * WIDTH);
    long long int shape_index = 0;
    long long int movement_index = 0;

    int *point_relationship = NULL;
    long long int rocks_placed_at_capture;
    long long int highest_point_at_capture;

    while (shape_index < MAX_ROCK_COUNT) {
        Shape *current_shape = get_pointer(shapes, (int)(shape_index % shapes->size));
        int x = 2;
        int y = (int) highest_point + 3;

        while (grid->size < y + current_shape->height) {
            int *empty_line = malloc(sizeof(int) * WIDTH);

            for (int i = 0; i < WIDTH; i++) {
                empty_line[i] = 0;
            }

            add_pointer(grid, empty_line);
        }

        while (1) {
            char current_movement = line[movement_index % length];
            x += ((current_movement == '>') && x + current_shape->width < WIDTH && !intersects(current_shape, x + 1, y, grid)) -
                    ((current_movement == '<') && x > 0 && !intersects(current_shape, x - 1, y, grid));
            movement_index++;

            if ((movement_index % length == 0) && (shape_index > (shapes->size * 8))) {
                get_highest_points(grid, highest_points);
                if (point_relationship == NULL) {
                    point_relationship = highest_points;
                    highest_points = malloc(sizeof(int) * WIDTH);
                    rocks_placed_at_capture = shape_index;
                    highest_point_at_capture = highest_point;
                } else {
                    int is_same = 1;
                    for (int j = 1; j < WIDTH; j++) {
                        if (highest_points[j] - highest_points[j - 1] !=
                            point_relationship[j] - point_relationship[j - 1]) {
                            is_same = 0;
                            break;
                        }
                    }
                    if (is_same) {
                        highest_point_offset =
                                ((MAX_ROCK_COUNT - rocks_placed_at_capture) / (shape_index - rocks_placed_at_capture) -
                                 1) * (highest_point - highest_point_at_capture);
                        shape_index =
                                (MAX_ROCK_COUNT - rocks_placed_at_capture) / (shape_index - rocks_placed_at_capture) *
                                (shape_index - rocks_placed_at_capture) + rocks_placed_at_capture;
                    }
                }
            }

            if (y <= 0 || intersects(current_shape, x, y - 1, grid)) {
                place_shape(current_shape, x, y, grid);
                if (highest_point < y + current_shape->height) {
                    highest_point = y + current_shape->height;
                }
                break;
            }
            y--;
        }

        shape_index++;

    }

    printf("%lld\n", highest_point + highest_point_offset);

    fclose(file);

    delete_pointerlist(grid);

    for (int i = 0; i < shapes->size; i++) {
        Shape *current_shape = get_pointer(shapes, i);
        for (int j = 0; j < current_shape->height; j++) {
            free(current_shape->values[j]);
        }
        free(current_shape->values);
    }

    delete_pointerlist(shapes);

    free(highest_points);

    free(point_relationship);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
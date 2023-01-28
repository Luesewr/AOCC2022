#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define REGION_SIZE 50

typedef struct Face {
    struct Face *neighbours[4];
    int direction_change[4];
    char grid[REGION_SIZE][REGION_SIZE];
    int x;
    int y;
} Face;

/**
 * Calculates the Manhattan distance between two points.
 * @param x1    The X-coordinate of point 1.
 * @param y1    The Y-coordinate of point 1.
 * @param x2    The X-coordinate of point 2.
 * @param y2    The Y-coordinate of point 2.
 * @return      The manhattan distance between the two given points.
 */
int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

/**
 * Returns the index of the next face if the next point is going off the current face.
 * @param x             The current X-coordinate within the cube face.
 * @param y             The current Y-coordinate within the cube face.
 * @param direction     The direction we're currently traveling in.
 * @return              The index of the next face in the neighbours or -1.
 */
int get_next_face(int x, int y, int direction) {
    if (direction == 0 && x == REGION_SIZE - 1) {
        return 0;
    } else if (direction == 1 && y == REGION_SIZE - 1) {
        return 1;
    } else if (direction == 2 && x == 0) {
        return 2;
    } else if (direction == 3 && y == 0) {
        return 3;
    }

    return -1;
}

/**
 * Calculates the next coordinates
 * @param face              The current face of the cube.
 * @param next_face_index   The index of the next cube face.
 * @param x                 The current X-coordinate.
 * @param y                 The current Y-coordinate.
 * @param direction         The current direction.
 */
void get_next_coordinate(Face *face, int next_face_index, int *x, int *y, int *direction) {
    int *coordinate_variable_one = *direction % 2 == 1 ? x : y;
    int *coordinate_variable_two = *direction % 2 == 1 ? y : x;
    int direction_change_sign = *direction <= 1 ? 1 : -1;

    if (next_face_index == -1) {
        *coordinate_variable_two = *coordinate_variable_two + direction_change_sign;
        return;
    }

    int direction_change = (face->direction_change[next_face_index] + 4) % 4;

    int x_possibilities[4] = {*x, (REGION_SIZE - 1), (REGION_SIZE - 1) - *x, 0};

    int y_possibilities[4] = {*y, 0, (REGION_SIZE - 1) - *y, (REGION_SIZE - 1)};

    int *coordinate_possibilities = *direction % 2 == 1 ? x_possibilities : y_possibilities;
    int offset = *direction < 1 || *direction > 2 ? 1 : -1;

    *coordinate_variable_one = coordinate_possibilities[(4 + direction_change * direction_change_sign) % 4];
    *coordinate_variable_two = coordinate_possibilities[(4 + offset + direction_change * direction_change_sign) % 4];

    *direction += direction_change % 4;
    *direction %= 4;
}

/*
 * Day 22, Part 2
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

    file = fopen("../days/day22/input_day22.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables:
     *  line, to store the input line.
     */
    char line[160];

    int y = 0;

    PointerList *faces = initialize_pointerlist_of_capacity(6);

    while (fgets(line, 160, file) != NULL) {
        if (line[0] == '\n' || line[0] == '\r') {
            break;
        }

        int x = 0;

        while (line[x] != '\n' && line[x] != '\r') {

            if (line[x] != ' ') {

                int found = 0;
                Face *current_face;

                for (int i = 0; i < faces->size; i++) {
                    current_face = get_pointer(faces, i);
                    if ((current_face->x == (x / REGION_SIZE)) && (current_face->y == (y / REGION_SIZE))) {
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    Face *new_face = malloc(sizeof(Face));
                    new_face->x = x / REGION_SIZE;
                    new_face->y = y / REGION_SIZE;

                    for (int i = 0; i < 4; i++) {
                        new_face->neighbours[i] = NULL;
                    }

                    current_face = new_face;
                    add_pointer(faces, new_face);
                }

                for (int i = 0; i < REGION_SIZE; i++) {
                    current_face->grid[y % REGION_SIZE][(x + i) % REGION_SIZE] = line[x + i];
                }

            }

            x += REGION_SIZE;

        }

        y++;
    }

    int faces_connected = 0;

    //Connect direct edges.
    for (int i = 0; i < faces->size; i++) {
        Face *current_face = get_pointer(faces, i);
        for (int j = 0; j < faces->size; j++) {
            Face *current_inner_face = get_pointer(faces, j);
            if (manhattan_distance(current_face->x, current_face->y, current_inner_face->x, current_inner_face->y) == 1) {
                if (current_inner_face->x > current_face->x) {
                    current_face->neighbours[0] = current_inner_face;
                    current_face->direction_change[0] = 0;
                    faces_connected++;
                }
                if (current_inner_face->y > current_face->y) {
                    current_face->neighbours[1] = current_inner_face;
                    current_face->direction_change[1] = 0;
                    faces_connected++;
                }
                if (current_inner_face->x < current_face->x) {
                    current_face->neighbours[2] = current_inner_face;
                    current_face->direction_change[2] = 0;
                    faces_connected++;
                }
                if (current_inner_face->y < current_face->y) {
                    current_face->neighbours[3] = current_inner_face;
                    current_face->direction_change[3] = 0;
                    faces_connected++;
                }
            }
        }
    }

    //Fold the cube.
    while (faces_connected < faces->size * 4) {
        for (int i = 0; i < faces->size; i++) {
            Face *current_face = get_pointer(faces, i);
            for (int current_direction = 0; current_direction < 4; current_direction++) {
                if (current_face->neighbours[current_direction] == NULL) {
                    Face *left_neighbour = current_face->neighbours[(current_direction + 4 - 1) % 4];
                    Face *right_neighbour = current_face->neighbours[(current_direction + 4 + 1) % 4];

                    if (left_neighbour != NULL) {
                        int direction_change = current_face->direction_change[(current_direction + 4 - 1) % 4];
                        Face *left_diagonal_neighbour = left_neighbour->neighbours[(current_direction + direction_change + 4) % 4];
                        if (left_diagonal_neighbour != NULL) {
                            int nested_direction_change = direction_change + left_neighbour->direction_change[(current_direction + 4 + direction_change) % 4];
                            current_face->neighbours[current_direction] = left_diagonal_neighbour;
                            left_diagonal_neighbour->neighbours[(current_direction + nested_direction_change + 1 + 4) % 4] = current_face;
                            current_face->direction_change[current_direction] = (nested_direction_change - 1 + 4) % 4;
                            left_diagonal_neighbour->direction_change[(current_direction + nested_direction_change + 1 + 4) % 4] = ((4 - (nested_direction_change - 1)) + 4) % 4;
                            faces_connected += 2;
                        }
                    } else if (right_neighbour != NULL) {
                        int direction_change = current_face->direction_change[(current_direction + 4 + 1) % 4];
                        Face *right_diagonal_neighbour = right_neighbour->neighbours[(current_direction + direction_change + 4) % 4];
                        if (right_diagonal_neighbour != NULL) {
                            int nested_direction_change = direction_change + right_neighbour->direction_change[(current_direction + 4 + direction_change) % 4];
                            current_face->neighbours[current_direction] = right_diagonal_neighbour;
                            right_diagonal_neighbour->neighbours[(current_direction + nested_direction_change - 1 + 4) % 4] = current_face;
                            current_face->direction_change[current_direction] = (nested_direction_change + 1 + 4) % 4;
                            right_diagonal_neighbour->direction_change[(current_direction + nested_direction_change - 1 + 4) % 4] = ((4 - (nested_direction_change + 1)) + 4) % 4;
                            faces_connected += 2;
                        }
                    }
                }
            }
        }
    }

    char input[5700];

    if (fgets(input, 5700, file) == NULL) {
        return 1;
    }

    char *ptr = input;

    int direction = 0;
    int x = 0;
    y = 0;
    Face *current_face = get_pointer(faces, 0);
    int is_number = 1;

    while (ptr[0] != '\n' && ptr[0] != '\r') {
        direction = (direction + 4) % 4;
        if (is_number) {
            int amount = (int) strtol(ptr, &ptr, 10);
            for (int i = 0; i < amount; i++) {
                int next_face_index = get_next_face(x, y, direction);
                int prev_x = x;
                int prev_y = y;
                int prev_direction = direction;
//                if (direction % 2 == 0) {
                get_next_coordinate(current_face, next_face_index, &x, &y, &direction);
//                } else {
//                    get_next_y(current_face, next_face_index, &x, &y, &direction);
//                }

                Face *next_face;
                if (next_face_index != -1) {
                    next_face = current_face->neighbours[next_face_index];
                } else {
                    next_face = current_face;
                }

                if (next_face->grid[y][x] == '#') {
                    x = prev_x;
                    y = prev_y;
                    direction = prev_direction;
                    break;
                } else {
                    current_face = next_face;
                }
            }
        } else {
            if (ptr[0] == 'R') {
                direction = (direction + 1) % 4;
            } else {
                direction = (direction + 4 - 1) % 4;
            }
            ptr++;
        }
        is_number = !is_number;
    }

    int password = 1000 * (y + 1 + REGION_SIZE * current_face->y) + 4 * (x + 1 + REGION_SIZE * current_face->x) + direction;

    printf("%d\n", password);

    fclose(file);

    delete_pointerlist(faces);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
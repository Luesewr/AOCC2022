//
// Created by Laure on 2023-01-06.
//

#ifndef AOCC_DAY17_FUNCTIONS_H
#define AOCC_DAY17_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef struct Shape {
    int width;
    int height;
    int **values;
} Shape;

void parse_shapes(PointerList *shapes, int width);

int intersects(Shape *shape, int sx, int sy, PointerList *grid);

void place_shape(Shape *shape, int sx, int sy, PointerList *grid);

void get_highest_points(PointerList *grid, int *highest_points, int width);

void calculate_and_print_height(PointerList *shapes, int width, long long int max_rock_count);

//void print_grid(PointerList *grid);

#endif //AOCC_DAY17_FUNCTIONS_H

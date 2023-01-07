//
// Created by Laure on 2023-01-07.
//

#ifndef AOCC_DAY18_FUNCTIONS_H
#define AOCC_DAY18_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef struct Point3D {
    int x;
    int y;
    int z;
} Point3D;

void parse_3D_grid(PointerList *points, PointerList *Grid3D, int max_size, int initialisation_value);

void set_3D_int(PointerList *Grid3D, int x, int y, int z, int value);

int get_3D_int(PointerList *Grid3D, int x, int y, int z);

void delete_3D_grid(PointerList *Grid3D);

int count_non_neighbours(PointerList *Grid3D, Point3D *point);

int check_coords(int x, int y, int z, int max_size);

void flood_outside(PointerList *Grid3D, PointerList *stack, int x, int y, int z, int max_size);

void floodfill_from_origin(PointerList *Grid3D, int max_size);

int calculate_surface_area(PointerList *points, PointerList *Grid3D);

#endif //AOCC_DAY18_FUNCTIONS_H

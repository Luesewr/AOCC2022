//
// Created by Laure on 2022-12-10.
//

#ifndef AOCC_DAY9_FUNCTIONS_H
#define AOCC_DAY9_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef struct point {
    int x;
    int y;
} point;

typedef struct pointSet {
    int bucket_amount;
    PointerList ** buckets;
    int size;
} pointSet;

int signum(int x);

point *create_point(int x, int y);

pointSet *create_point_set(int buckets_amount);

int pointHashCode(int x, int y);

long long point_to_int(int x, int y);

void add_to_set(pointSet *set, int x, int y);

void delete_point_set(pointSet * set);

#endif //AOCC_DAY9_FUNCTIONS_H

//
// Created by Laure on 2022-12-15.
//

#ifndef AOCC_DAY14_FUNCTIONS_H
#define AOCC_DAY14_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef struct pointSet {
    int bucket_amount;
    PointerList ** buckets;
    int size;
} pointSet;

int parse_input(pointSet *set);

int signum(int x);

pointSet *create_point_set(int buckets_amount);

int pointHashCode(int x, int y);

long long point_to_int(int x, int y);

void add_to_set(pointSet *set, int x, int y);

int set_contains(pointSet *set, int x, int y);

void add_directly_to_set(pointSet *set, int x, int y);

void delete_point_set(pointSet * set);

#endif //AOCC_DAY14_FUNCTIONS_H

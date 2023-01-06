#ifndef AOCC_DAY15_FUNCTIONS_H
#define AOCC_DAY15_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef struct point {
    int x;
    int y;
} point;

typedef struct sensor {
    point *position;
    int distance;
} sensor;

void parse_input(PointerList *sensors);

point *create_point(int x, int y);

int distance(point *p1, point *p2);

sensor *create_sensor(point *position, point *beacon);

#endif //AOCC_DAY15_FUNCTIONS_H

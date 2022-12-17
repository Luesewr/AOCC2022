#include "day15_functions.h"
#include <stdio.h>
#include <stdlib.h>

void parse_input(PointerList *sensors) {

    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day15/input_day15.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        exit(1);
    }

    char line[80];

    /*
     * Loop over the entire input.
     */
    while (fgets(line, 80, file) != NULL) {
        char *ptr = line + 12;
        int sensor_x = (int) strtol(ptr, &ptr, 10);
        ptr += 4;
        int sensor_y = (int) strtol(ptr, &ptr, 10);

        ptr += 25;
        int beacon_x = (int) strtol(ptr, &ptr, 10);
        ptr += 4;
        int beacon_y = (int) strtol(ptr, &ptr, 10);

        add_pointer(sensors, create_sensor(create_point(sensor_x, sensor_y), create_point(beacon_x, beacon_y)));
    }

    fclose(file);
}

/**
 * Creates a sensor at (x; y).
 * @param x     The horizontal position.
 * @param y     The vertical position.
 * @return a pointer to the created sensor.
 */
point *create_point(int x, int y) {
    point *new_point = malloc(sizeof(point));

    new_point->x = x;
    new_point->y = y;

    return new_point;
}

/**
 * Calculates the Manhattan distance between 2 points.
 * @param p1    point 1.
 * @param p2    point 2.
 * @return the Manhattan distance between the 2 points.
 */
int distance(point *p1, point *p2) {
    return abs(p1->x - p2->x) + abs(p1->y - p2->y);
}

/**
 * Create a sensor.
 * @param position  the position of the sensor.
 * @param beacon    the position of the beacon.
 * @return a pointer to the created sensor.
 */
sensor *create_sensor(point *position, point *beacon) {
    sensor *new_sensor = malloc(sizeof(sensor));

    new_sensor->position = position;
    new_sensor->distance = distance(position, beacon);

    free(beacon);

    return new_sensor;
}
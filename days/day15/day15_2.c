#include "day15_functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_COORDINATE 4000000
//#define MAX_COORDINATE 20

int current_line;

/*
 * Day 15, Part 2
 */
int main() {
    /*
     * Set up timer.
     */
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Make variables:
     *  sensors, a hashset containing all the sensors.
     */
    PointerList *sensors = initialize_pointerlist();

    /*
     * Parses the sensors in the input into the sensors list.
     */
    parse_input(sensors);

    /*
     * Loop over the lines within the allowed coordinates.
     */
    for (current_line = 0; current_line <= MAX_COORDINATE; current_line++) {

        /*
         * Keep track of the current position.
         */
        int cur_x = 0;

        /*
         * Skip over the covered ranges of the sensors as long as it still skips.
         */
        int old_position;
        do {

            old_position = cur_x;
            /*
             * Look through all the sensors to see if the current x value falls within the range of any.
             */
            for (int i = 0; i < sensors->size && cur_x <= MAX_COORDINATE; i++) {

                sensor *cur_sensor = get_pointer(sensors, i);
                int reach = (cur_sensor->distance - abs(cur_sensor->position->y - current_line));

                /*
                 * If it falls within the range of a sensor skip to just beyond the reach of the sensor.
                 */
                if (abs(cur_x - cur_sensor->position->x) <= reach) {
                    cur_x = cur_sensor->position->x + reach + 1;
                }
            }
        } while (old_position != cur_x && cur_x <= MAX_COORDINATE);

        /*
         * If the horizontal position is still within the allowed coordinates after skipping the ranges,
         * that means that the horizontal position is not covered by any sensor and is in fact the location
         * we're looking for.
         */
        if (cur_x <= MAX_COORDINATE) {

            /*
             * Calculate the tuning frequency, print it and stop looking.
             */
            long long int tuning_frequency = cur_x;
            tuning_frequency *= 4000000;
            tuning_frequency += current_line;
            printf("%lld\n", tuning_frequency);
            break;
        }
    }

    /*
     * Deallocate all allocated memory.
     */

    for (int i = 0; i < sensors->size; i++) {
        sensor *cur_sensor = get_pointer(sensors, i);
        free(cur_sensor->position);
    }
    delete_pointerlist(sensors);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
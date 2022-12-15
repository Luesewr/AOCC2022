#include "day15_functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LINE_Y 2000000
//#define LINE_Y 10

/**
 * Predicate function to see if a sensor is too far from the LINE_Y.
 * @param data  a void pointer to a sensor.
 * @return      1 if the sensor is too far from LINE_Y, otherwise return 0.
 */
int tooFar(void *data) {
    sensor *s = data;
    return abs(s->position->y - LINE_Y) > s->distance;
}

/*
 * Day 15, Part 1
 */
int main() {
    /*
     * Set up timer.
     */
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Make variables:
     *  sensors, a list containing all the sensors.
     */
    PointerList *sensors = initialize_pointerlist();

    /*
     * Parses the sensors in the input into the sensors list.
     */
    parse_input(sensors);

    /*
     * Removes all sensors that are too far away from the line to save on calculation time.
     */
    PointerList *filtered_out = remove_pointer_if(sensors, &tooFar);

    /*
     * Deallocates all the sensors that were filtered out.
     */
    for (int i = 0; i < filtered_out->size; i++) {
        sensor *cur_sensor = get_pointer(filtered_out, i);
        free(cur_sensor->position);
    }

    delete_pointerlist(filtered_out);

    /*
     * Find the lowest and highest indices that are covered by sensors on LINE_Y.
     */

    int lowest_covered_index = 0x7fffffff, highest_covered_index = -0x7fffffff;

    for (int i = 0; i < sensors->size; i++) {
        sensor *cur_sensor = get_pointer(sensors, i);
        int smallest_index = cur_sensor->position->x - (cur_sensor->distance - abs(cur_sensor->position->y - LINE_Y));
        if (smallest_index < lowest_covered_index) {
            lowest_covered_index = smallest_index;
        }
        int biggest_index = cur_sensor->position->x + (cur_sensor->distance - abs(cur_sensor->position->y - LINE_Y));
        if (biggest_index > highest_covered_index) {
            highest_covered_index = biggest_index;
        }
    }

    /*
     * Look through Y_LINE from the lowest to the highest covered index and count how much is covered.
     */

    int cur_x = lowest_covered_index;
    int amount_covered = 0;

    while (cur_x <= highest_covered_index) {

        /*
         * Skip over the covered ranges of the sensors as long as it still skips.
         */
        int old_position;
        do {
            old_position = cur_x;

            /*
             * Look through all the sensors to see if the current x value falls within the range of any.
             */
            for (int i = 0; i < sensors->size; i++) {

                sensor *cur_sensor = get_pointer(sensors, i);
                int reach = (cur_sensor->distance - abs(cur_sensor->position->y - LINE_Y));

                /*
                 * If it falls within the range of a sensor, calculate how far we have to skip to get out of the range
                 * of the sensor. Add the amount of cells that we have to move to the amount covered, since all of
                 * it was in range of a sensor. Then skip to just beyond the reach of the sensor.
                 */
                if (abs(cur_x - cur_sensor->position->x) <= reach) {
                    int end_of_reach = cur_sensor->position->x + reach;
                    amount_covered += end_of_reach + 1 - cur_x;
                    cur_x = end_of_reach + 1;
                }
            }
        } while (old_position != cur_x);
        /*
         * If we're not in a space that is covered by a sensor, move over one cell to the right.
         */
        cur_x++;
    }

    /*
     * Correct the amount covered.
     */
    if (amount_covered > 0) {
        amount_covered--;
    }

    /*
     * Print the amount of covered cells.
     */
    printf("%d\n", amount_covered);

    /*
     * Deallocate the remaining allocated memory.
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
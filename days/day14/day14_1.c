#include "day14_functions.h"
#include <stdio.h>
#include <time.h>

#define SAND_SOURCE_X 500
#define SAND_SOURCE_Y 0

#define SET_BUCKETS 500

/*
 * Day 14, Part 1
 */
int main() {

    /*
     * Set up timer.
     */
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Make variables:
     *  set, a hashset containing all the points.
     *  highestY, stores the highest height value over all the points.
     *  set_size_before_sand, speaks for itself.
     *  cur_sand_x, keeps track of the current horizontal sand position.
     *  cur_sand_y, keeps track of the current vertical sand position.
     */

    pointSet *set = create_point_set(SET_BUCKETS);

    int highestY = parse_input(set);

    int set_size_before_sand = set->size;

    int cur_sand_x;
    int cur_sand_y = 0;

    /*
     * Keep creating new sand until a sand unit reaches the bottom.
     */
    while (cur_sand_y != highestY) {

        /*
         * Set the sand position to the position of the sand source.
         */
        cur_sand_x = SAND_SOURCE_X;
        cur_sand_y = SAND_SOURCE_Y;

        /*
         * Simulate the current sand unit.
         */
        while (cur_sand_y != highestY) {

            /*
             * Move a sand unit down according to the rules until it can't move down anymore,
             * then place it down and add that point to the set.
             */
            int next_y = cur_sand_y + 1;

            if (!set_contains(set, cur_sand_x, next_y)) {
                cur_sand_y = next_y;
            } else if (!set_contains(set, cur_sand_x - 1, next_y)) {
                cur_sand_y = next_y;
                cur_sand_x--;
            } else if (!set_contains(set, cur_sand_x + 1, next_y)) {
                cur_sand_y = next_y;
                cur_sand_x++;
            } else {
                add_directly_to_set(set, cur_sand_x, cur_sand_y);
                break;
            }
        }
    }

    /*
     * Print the amount of sand added.
     */
    int set_size_after_sand = set->size;

    printf("%d\n", set_size_after_sand - set_size_before_sand);

    /*
     * Delete allocated area.
     */
    delete_point_set(set);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
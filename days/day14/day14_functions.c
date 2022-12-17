#include "day14_functions.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Fill the given set with the points of the lines in the input while keeping track of the highest y-value.
 * @param set   the set to fill with points.
 * @return      the highest y-value in all the points.
 */
int parse_input(pointSet *set) {

    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day14/input_day14.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables for the input line and the highest y-value.
     */
    char line[300];
    int highestY = 0;

    /*
     * Loop over the entire input.
     */
    while (fgets(line, 300, file) != NULL) {

        /*
         * Create a pointer to a point within the input line.
         */
        char *ptr = line;

        /*
         * Get the first point in the line.
         */
        int prevX = (int) strtol(ptr, &ptr, 10);
        ptr += 1;
        int prevY = (int) strtol(ptr, &ptr, 10);

        /*
         * Add the first point to the point set.
         */
        add_to_set(set, prevX, prevY);

        /*
         * Try to update the highest y-value.
         */
        if (prevY > highestY) {
            highestY = prevY;
        }

        /*
         * Loop over the line until you get to the end of the line.
         */
        while (ptr[0] != '\n' && ptr[0] != '\r') {

            /*
             * Read the next point.
             */
            ptr += 4;
            int x = (int) strtol(ptr, &ptr, 10);
            ptr += 1;
            int y = (int) strtol(ptr, &ptr, 10);

            /*
             * Compare the current point to the previous point and add all the points in between to the set.
             */
            if (x == prevX) {
                int dy = signum(y - prevY);

                for (; prevY != y + dy; prevY += dy) {
                    add_to_set(set, prevX, prevY);

                    if (prevY > highestY) {
                        highestY = prevY;
                    }
                }

                prevY = y;
            } else {
                int dx = signum(x - prevX);
                prevX += dx;

                for (; prevX != x + dx; prevX += dx) {
                    add_to_set(set, prevX, prevY);

                    if (prevY > highestY) {
                        highestY = prevY;
                    }
                }

                prevX = x;
            }
        }
    }

    fclose(file);

    return highestY;
}

/**
 * Gets the signum of an int.
 * @param x     The input.
 * @return      x <  0 -> -1
 *              x == 0 ->  0
 *              x >  0 ->  1
 */
int signum(int x) {
    return x < 0 ? -1 : (x > 0);
}

/**
 * Initializes a point set.
 * @param buckets_amount    The amount of buckets the set should have.
 * @return a pointer to the created point set.
 */
pointSet *create_point_set(int buckets_amount) {
    pointSet *new_point_set = malloc(sizeof(pointSet));
    new_point_set->size = 0;
    new_point_set->bucket_amount = buckets_amount;
    new_point_set->buckets = malloc(sizeof(PointerList *) * buckets_amount);

    for (int i = 0; i < buckets_amount; i++) {
        new_point_set->buckets[i] = initialize_pointerlist();
    }

    return new_point_set;
}

/**
 * Creates a hashcode for a location.
 * @param x     The horizontal position.
 * @param y     The vertical position.
 * @return the hashcode of the location.
 */
int pointHashCode(int x, int y) {
    return abs(x) * 200 + abs(y);
}

/**
 * Maps a location to one unique big number.
 * @param x     The horizontal position.
 * @param y     The vertical position.
 * @return a unique number which contains all the position information.
 */
long long int point_to_int(int x, int y) {
    long long int px = abs(x);
    long long int py = y;
    return ((py < 0) << 31) | (py << 16) | ((x < 0) << 15) | px;
}

/**
 * Adds a point (x, y) to the point set.
 * @param set   The hashset of points.
 * @param x     The horizontal position.
 * @param y     The vertical position.
 */
void add_to_set(pointSet *set, int x, int y) {
    int index = pointHashCode(x, y) % set->bucket_amount;
    void *point_int = (void *) point_to_int(x, y);
    PointerList *bucket = set->buckets[index];
    if (!contains(bucket, point_int)) {
        add_pointer(bucket, point_int);
        set->size++;
    }
}

/**
 * Checks whether a point at (x, y) is already in the set.
 * @param set   a point set containing all the points.
 * @param x     the horizontal position of the point to check.
 * @param y     the vertical position of the point to check.
 * @return  1 if the set contains the point, otherwise return 0.
 */
int set_contains(pointSet *set, int x, int y) {
    int index = pointHashCode(x, y) % set->bucket_amount;
    void *point_int = (void *) point_to_int(x, y);
    PointerList *bucket = set->buckets[index];
    return contains(bucket, point_int);
}

/**
 * Adds a point at (x, y) without checking if it's already in the list, use responsibly.
 * @param set   a point set containing all the points.
 * @param x     the horizontal position of the point to check.
 * @param y     the vertical position of the point to check.
 */
void add_directly_to_set(pointSet *set, int x, int y) {
    int index = pointHashCode(x, y) % set->bucket_amount;
    void *point_int = (void *) point_to_int(x, y);
    PointerList *bucket = set->buckets[index];
    add_pointer(bucket, point_int);
    set->size++;
}

/**
 * Deletes a point set.
 * @param set   The set to delete.
 */
void delete_point_set(pointSet * set) {
    for (int i = 0; i < set->bucket_amount; i++) {
        delete_pointerlist_not_pointers(set->buckets[i]);
    }
    free(set->buckets);
    free(set);
}
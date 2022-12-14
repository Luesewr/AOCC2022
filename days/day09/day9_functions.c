#include <stdlib.h>
#include "../../library/pointerList.h"
#include "day9_functions.h"

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
 * Creates a point at (x; y).
 * @param x     The horizontal position.
 * @param y     The vertical position.
 * @return a pointer to the created point.
 */
point *create_point(int x, int y) {
    point *new_point = malloc(sizeof(point));
    new_point->x = x;
    new_point->y = y;
    return new_point;
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
    return abs(x * y);
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
 * Adds a point (x; y) to the point set.
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

int isPointerlist(PointerList *list, int index) {
    int y = get_int(list, index);
    return (y < 0 || y > 10);
}

//PointerList *deepCopy(PointerList *list) {
//    PointerList *copy = initialize_pointerlist();
//    for (int i = 0; i < list->size; i++) {
//        if (isPointerlist(list, i)) {
//            add_pointer(copy, deepCopy(get_pointer(list, i)));
//        } else {
//            add_int(copy, get_int(list, i));
//        }
//    }
//    return copy;
//}

int getFirstElement(PointerList *list) {
    if (list->size == 0) {
        return -1;
    } else if (isPointerlist(list, 0)) {
        return getFirstElement(get_pointer(list, 0));
    } else {
        return get_int(list, 0);
    }
}

int packetComparator(void *one, void *two) {
    /*
     * Create back tracking lists for both elements in the pair.
     */
    PointerList *back_track_one = initialize_pointerlist();
    PointerList *back_track_two = initialize_pointerlist();
    PointerList *cur_one = one;
    PointerList *cur_two = two;

//    printf("Printing 1: ");
//    print_ints(cur_one);
//    printf("Printing 2: ");
//    print_ints(cur_two);
    /*
     * Create a backtracking list for the index.
     */
    PointerList *index_back_track = initialize_pointerlist();

    /*
     * Get the base lists for both elements in the pair.
     */

    /*
     * Create variables to keep track of where we are in the current list as well as keeping track of
     * whether everything in the pair is in the right order.
     */
    int index = 0;
    int correct_order = 1;

    /*
     * Analyze the pair.
     */
    while (1) {

        /*
         * If we've reached the end of current list one.
         */
        if (index >= cur_one->size) {
            /*
             * Break if we're at the end of the pair, because there's nothing to backtrack to.
             */
            if (back_track_one->size == 0 || cur_one->size < cur_two->size) {
                break;
            }
                /*
                 * Otherwise, get the last lists from the backtracking lists and set the current lists to that.
                 * As well as restoring the index that we left off with in the list.
                 */
            else {
                cur_one = remove_at(back_track_one, back_track_one->size - 1);
                cur_two = remove_at(back_track_two, back_track_two->size - 1);
                index = remove_last_int(index_back_track) + 1;

//                    printf("]");
                continue;

            }
        }
            /*
             * If we've reached the end of list 2 and not list 1, the order isn't correct and the analysing is stopped.
             */
        else if (index >= cur_two->size) {
            correct_order = 0;
//                printf("\nReached end of list 2 first\n");
            break;
        }

        /*
         * Check if the current values in the current lists are lists or not.
         */
        int is_one_list = isPointerlist(cur_one, index);
        int is_two_list = isPointerlist(cur_two, index);

        /*
         * If only one of the values is a list.
         */
        if (is_one_list ^ is_two_list) {
            if (is_one_list) {
                int result = getFirstElement(cur_one);
                if (result >= 0 && result > get_int(cur_two, index)) {
                    correct_order = 0;
                    break;
                }
            } else {
                int result = getFirstElement(cur_two);
                if (result == -1 || result < get_int(cur_one, index)) {
                    correct_order = 0;
                    break;
                }
            }
            index++;
        } else if (!is_one_list && !is_two_list) {
//                printf("index: %d. %d, %d\n", index, get_int(cur_one, index), get_int(cur_two, index));
            int cur_one_int = get_int(cur_one, index);
            int cur_two_int = get_int(cur_two, index);
//                printf("{%d, %d}", cur_one_int, cur_two_int);
            if (cur_one_int > cur_two_int) {
                correct_order = 0;
//                    printf("\nIncorrect Integer order\n");
                break;
            } else if (cur_one_int == cur_two_int) {
                index++;
            } else {
                break;
            }
        } else {
            add_pointer(back_track_one, cur_one);
            add_pointer(back_track_two, cur_two);
            cur_one = get_pointer(cur_one, index);
            cur_two = get_pointer(cur_two, index);
            add_int(index_back_track, index);
            index = 0;
//                printf("[");
        }
    }
//    printf("Returning %d\n", !correct_order);
    return !correct_order;
}

/*
 * Day 13, Part 2
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day13/testInput_day13.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  grid, a 2-dimensional list containing all the cells in the input.
     *  y, to keep track of the row in the input.
     *  start_cell, to store the cell from where we start looking.
     *  end_cell, to store the cell where we need to go.
     */
    char line[230];

    PointerList *packets = initialize_pointerlist();

    int i = 0;

    /*
     * Loop over the input file.
     */
    while (fgets(line, 230, file) != NULL) {
        i++;
        if (line[0] == '\n' || line[1] == '\n') {
            continue;
        }
        PointerList *back_track = initialize_pointerlist();
        char *ptr = line + 1;
        PointerList *current = initialize_pointerlist();
        while (ptr[0] != '\n') {
            if (ptr[0] == '[') {
                add_pointer(back_track, current);
                PointerList *inner = initialize_pointerlist();
                add_pointer(current, inner);
                current = inner;
                ptr++;
            } else if (ptr[0] == ']') {
                if (back_track->size == 0) {
                    break;
                } else {
                    current = remove_at(back_track, back_track->size - 1);
                }
                ptr++;
            } else if (ptr[0] == ',') {
                ptr++;
            } else {
                add_int(current, (int) strtol(ptr, &ptr, 10));
            }
        }
        add_pointer(packets, current);
        delete_pointerlist_not_pointers(back_track);
    }

    PointerList *custom_packet_2 = initialize_pointerlist();
    PointerList *packet_2 = initialize_pointerlist();
    add_int(packet_2, 2);
    add_pointer(custom_packet_2, packet_2);

    PointerList *custom_packet_6 = initialize_pointerlist();
    PointerList *packet_6 = initialize_pointerlist();
    add_int(packet_6, 6);
    add_pointer(custom_packet_6, packet_6);

    add_pointer(packets, custom_packet_2);
    add_pointer(packets, custom_packet_6);

//    for (int j = 0; j < packets->size; j++) {
//        print_ints(get_pointer(packets, j));
//    }

    sort_pointers_with_comparator(packets, &packetComparator);

    for (int j = 0; j < packets->size; j++) {
        print_ints(get_pointer(packets, j));
    }

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

int isPointerList(PointerList *list, int index) {
    int y = get_int(list, index);
    return (y < 0 || y > 10);
}

PointerList *parseInput() {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day13/input_day13.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        exit(1);
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


    /*
     * Loop over the input file.
     */
    while (fgets(line, 230, file) != NULL) {
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

    return packets;
}

int packetComparator(void *one, void *two) {
    /*
     * Create back tracking lists for both elements in the pair.
     */
    PointerList *cur_one = one;
    PointerList *cur_two = two;

    /*
     * Get the base lists for both elements in the pair.
     */

    /*
     * Create variables to keep track of where we are in the current list as well as keeping track of
     * whether everything in the pair is in the right order.
     */
    int index = 0;

    int max_length = cur_one->size > cur_two->size ? cur_one->size : cur_two->size;
    /*
     * Analyze the pair.
     */
    for (int i = 0; i < max_length; i++) {

        /*
         * If we've reached the end of current list one.
         */
        if (index >= cur_one->size) {
            return -1;
        } else if (index >= cur_two->size) {
            return 1;
        }

        /*
         * Check if the current values in the current lists are lists or not.
         */
        int is_one_list = isPointerList(cur_one, index);
        int is_two_list = isPointerList(cur_two, index);

        /*
         * If only one of the values is a list.
         */
        if (is_one_list ^ is_two_list) {
            PointerList *wrapper = initialize_pointerlist();
            int result;
            if (is_one_list) {
                add_int(wrapper, get_int(cur_two, index));
                result = packetComparator(get_pointer(cur_one, index), wrapper);
            } else {
                add_int(wrapper, get_int(cur_one, index));
                result = packetComparator(wrapper, get_pointer(cur_two, index));
            }
            delete_pointerlist(wrapper);
            if (result != 0) {
                return result;
            }
            index++;
        } else if (!is_one_list && !is_two_list) {
            int cur_one_int = get_int(cur_one, index);
            int cur_two_int = get_int(cur_two, index);

            if (cur_one_int > cur_two_int) {
                return 1;
            } else if (cur_one_int == cur_two_int) {
                index++;
            } else {
                return -1;
            }
        } else {
            int result = packetComparator(get_pointer(cur_one, index), get_pointer(cur_two, index));
            if (result != 0) {
                return result;
            }
            index++;
        }
    }
    return 0;
}

void deletePackets(PointerList *packets) {
    for (int i = 0; i < packets->size; i++) {
        if (isPointerList(packets, i)) {
            PointerList *current_list = get_pointer(packets, i);
            deletePackets(current_list);
            delete_pointerlist_not_pointers(current_list);
        } else {
            free(get_pointer(packets, i));
        }
    }
}
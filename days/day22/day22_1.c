#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

int get_next_x(PointerList *lines, PointerList *offsets, int x, int y, int direction) {
    int offset = get_int(offsets, y);

    int line_size = ((PointerList *) get_pointer(lines, y))->size;
    if (direction >= 1) {
        if (x - offset - 1 < 0) {
            return offset + line_size - 1;
        } else {
            return x - 1;
        }
    } else {
        if (x - offset + 1 >= line_size) {
            return offset;
        } else {
            return x + 1;
        }
    }
}

int get_next_y(PointerList *lines, PointerList *offsets, int x, int y, int direction) {
    if (direction >= 2) {
        int line_size;
        if (y - 1 >= 0) {
            line_size = ((PointerList *) get_pointer(lines, y - 1))->size;
        }
        int offset = get_int(offsets, y - 1);
        if (y - 1 < 0 || offset > x || offset + line_size <= x) {
            int search_y = y + 1;
            while (search_y < lines->size && (get_int(offsets, search_y) <= x) && (get_int(offsets, search_y) + ((PointerList *) get_pointer(lines, search_y))->size) > x) {
                search_y++;
            }
            return search_y - 1;
        } else {
            return y - 1;
        }
    } else {
        int line_size;
        if (y + 1 < lines->size) {
            line_size = ((PointerList *) get_pointer(lines, y + 1))->size;
        }
        int offset = get_int(offsets, y + 1);
        if (y + 1 >= lines->size || offset > x || offset + line_size <= x) {
            int search_y = y - 1;
            while (search_y >= 0 && (get_int(offsets, search_y) <= x) && (get_int(offsets, search_y) + ((PointerList *) get_pointer(lines, search_y))->size) > x) {
                search_y--;
            }
            return search_y + 1;
        } else {
            return y + 1;
        }
    }
}

/*
 * Day 22, Part 1
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

    file = fopen("../days/day22/input_day22.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables:
     *  line, to store the input line.
     */
    char line[160];

    PointerList *offsets = initialize_pointerlist();
    PointerList *lines = initialize_pointerlist();

    while (fgets(line, 160, file) != NULL) {
        if (line[0] == '\n') {
            break;
        }

        PointerList *current_line = initialize_pointerlist();

        add_pointer(lines, current_line);

        int i = 0;

        while (line[i] == ' ') {
            i++;
        }

        add_int(offsets, i);

        int j = i;

        while (line[j] != '\n') {
            add_int(current_line, line[j]);
            j++;
        }
    }

    char input[5700];

    if (fgets(input, 5700, file) == NULL) {
        return 1;
    }

    char *ptr = input;

    int direction = 0;
    int x = get_int(offsets, 0);
    int y = 0;
    int is_number = 1;

    while (ptr[0] != '\n') {
        direction = (direction + 4) % 4;
        if (is_number) {
            int amount = (int) strtol(ptr, &ptr, 10);
            for (int i = 0; i < amount; i++) {
                if (direction % 2 == 0) {
                    int next_x = get_next_x(lines, offsets, x, y, direction);
                    PointerList *current_line = get_pointer(lines, y);
                    if (get_int(current_line, next_x - get_int(offsets, y)) == '#') {
                        break;
                    }
                    x = next_x;
                } else {
                    int next_y = get_next_y(lines, offsets, x, y, direction);
                    PointerList *current_line = get_pointer(lines, next_y);
                    if (get_int(current_line, x - get_int(offsets, next_y)) == '#') {
                        break;
                    }
                    y = next_y;
                }
            }
        } else {
            if (ptr[0] == 'R') {
                direction++;
            } else {
                direction--;
            }
            ptr++;
        }
        is_number = !is_number;
    }

    int password = 1000 * (y + 1) + 4 * (x + 1) + direction;

    printf("%d\n", password);

    fclose(file);

    delete_pointerlist(offsets);

    for (int i = 0; i < lines->size; i++) {
        delete_pointerlist(get_pointer(lines, i));
    }

    delete_pointerlist_not_pointers(lines);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"

typedef struct Elf {
    int x;
    int y;
} Elf;

typedef struct Proposal {
    Elf *elf;
    int px;
    int py;
    int ox;
    int oy;
} Proposal;

/*
 * Day 23, Part 2
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

    file = fopen("../days/day23/input_day23.txt", "r");

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
    char line[100];

    int x, y;

    PointerList *elves = initialize_pointerlist();
    y = 0;

    while (fgets(line, 100, file) != NULL) {
        x = 0;

        while (line[x] != '\n' && line[x] != '\r') {
            if (line[x] == '#') {
                Elf *new_elf = malloc(sizeof(Elf));
                new_elf->x = x;
                new_elf->y = y;

                add_pointer(elves, new_elf);
            }
            x++;
        }

        y++;
    }

    PointerList *elves_by_row = initialize_pointerlist_of_capacity(y);
    PointerList *elves_by_column = initialize_pointerlist_of_capacity(x);

    for (int i = 0; i < y; i++) {
        add_pointer(elves_by_row, initialize_pointerlist());
    }

    for (int i = 0; i < x; i++) {
        add_pointer(elves_by_column, initialize_pointerlist());
    }

    int moves;
    int round = 0;

    do {
        for (int i = 0; i < elves_by_row->size; i++) {
            PointerList *current_row = get_pointer(elves_by_row, i);
            current_row->size = 0;
        }

        for (int i = 0; i < elves_by_column->size; i++) {
            PointerList *current_column = get_pointer(elves_by_column, i);
            current_column->size = 0;
        }

        for (int i = 0; i < elves->size; i++) {
            Elf *current_elf = get_pointer(elves, i);
            PointerList *current_row = get_pointer(elves_by_row, (current_elf->y + elves_by_row->size * (round / elves_by_row->size + 1)) % elves_by_row->size);
            PointerList *current_column = get_pointer(elves_by_column, (current_elf->x + elves_by_column->size * (round / elves_by_column->size + 1)) % elves_by_column->size);
            add_pointer(current_row, current_elf);
            add_pointer(current_column, current_elf);
        }

        PointerList *proposals = initialize_pointerlist();
        moves = 0;

        for (int j = 0; j < elves->size; j++) {
            Elf *current_elf = get_pointer(elves, j);
            int move_options[4] = {1, 1, 1, 1};

            PointerList *upper_row = get_pointer(elves_by_row, (current_elf->y - 1 + elves_by_row->size * (round / elves_by_row->size + 1)) % elves_by_row->size);
            PointerList *lower_row = get_pointer(elves_by_row, (current_elf->y + 1 + elves_by_row->size * (round / elves_by_row->size + 1)) % elves_by_row->size);
            PointerList *left_column = get_pointer(elves_by_column, (current_elf->x - 1 + elves_by_column->size * (round / elves_by_column->size + 1)) % elves_by_column->size);
            PointerList *right_column = get_pointer(elves_by_column, (current_elf->x + 1 + elves_by_column->size * (round / elves_by_column->size + 1)) % elves_by_column->size);

            for (int k = 0; k < upper_row->size; k++) {
                Elf *current_inner_elf = get_pointer(upper_row, k);

                int dx = current_inner_elf->x - current_elf->x;
                int dy = current_elf->y - current_inner_elf->y;

                if (dx >= -1 && dx <= 1 && dy == 1) {
                    move_options[0] = 0;
                }
            }

            for (int k = 0; k < lower_row->size; k++) {
                Elf *current_inner_elf = get_pointer(lower_row, k);

                int dx = current_inner_elf->x - current_elf->x;
                int dy = current_elf->y - current_inner_elf->y;

                if (dx >= -1 && dx <= 1 && dy == -1) {
                    move_options[1] = 0;
                }
            }

            for (int k = 0; k < left_column->size; k++) {
                Elf *current_inner_elf = get_pointer(left_column, k);

                int dx = current_inner_elf->x - current_elf->x;
                int dy = current_elf->y - current_inner_elf->y;

                if (dy >= -1 && dy <= 1 && dx == -1) {
                    move_options[2] = 0;
                }
            }

            for (int k = 0; k < right_column->size; k++) {
                Elf *current_inner_elf = get_pointer(right_column, k);

                int dx = current_inner_elf->x - current_elf->x;
                int dy = current_elf->y - current_inner_elf->y;

                if (dy >= -1 && dy <= 1 && dx == 1) {
                    move_options[3] = 0;
                }
            }

            int options_amount = 4;

            for (int k = 0; k < 4; k++) {
                if (move_options[k] == 0) {
                    options_amount--;
                }
            }

            int px, py;
            int moved = 0;
            for (int k = 0; k < 4; k++) {
                if ((k + round) % 4 == 0 && move_options[0]) {
                    px = current_elf->x;
                    py = current_elf->y - 1;
                    moved = 1;
                    break;
                } else if ((k + round) % 4 == 1 && move_options[1]) {
                    px = current_elf->x;
                    py = current_elf->y + 1;
                    moved = 1;
                    break;
                } else if ((k + round) % 4 == 2 && move_options[2]) {
                    px = current_elf->x - 1;
                    py = current_elf->y;
                    moved = 1;
                    break;
                } else if ((k + round) % 4 == 3 && move_options[3]) {
                    px = current_elf->x + 1;
                    py = current_elf->y;
                    moved = 1;
                    break;
                }
            }

            if (!moved || (options_amount == 4)) {
                px = current_elf->x;
                py = current_elf->y;
            }

            for (int k = 0; k < proposals->size; k++) {
                Proposal *current_proposal = get_pointer(proposals, k);
                if (current_proposal->px == px && current_proposal->py == py) {
                    px = current_elf->x;
                    py = current_elf->y;
                    current_proposal->px = current_proposal->ox;
                    current_proposal->py = current_proposal->oy;
                    break;
                }
            }

            Proposal *new_proposal = malloc(sizeof(Proposal));

            new_proposal->px = px;
            new_proposal->py = py;
            new_proposal->ox = current_elf->x;
            new_proposal->oy = current_elf->y;
            new_proposal->elf = current_elf;

            add_pointer(proposals, new_proposal);
        }

        for (int j = 0; j < proposals->size; j++) {
            Proposal *current_proposal = get_pointer(proposals, j);

            if ((current_proposal->elf->x != current_proposal->px) || (current_proposal->elf->y != current_proposal->py)) {
                moves++;
            }

            current_proposal->elf->x = current_proposal->px;
            current_proposal->elf->y = current_proposal->py;
        }

        delete_pointerlist(proposals);

        round++;

    } while (moves > 0);

    printf("%d\n", round);

    for (int i = 0; i < elves_by_row->size; i++) {
        PointerList *current_row = get_pointer(elves_by_row, i);
        delete_pointerlist_not_pointers(current_row);
    }

    delete_pointerlist_not_pointers(elves_by_row);

    for (int i = 0; i < elves_by_column->size; i++) {
        PointerList *current_column = get_pointer(elves_by_column, i);
        delete_pointerlist_not_pointers(current_column);
    }

    delete_pointerlist_not_pointers(elves_by_column);

    delete_pointerlist(elves);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
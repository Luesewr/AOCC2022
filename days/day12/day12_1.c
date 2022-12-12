#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

typedef struct Cell {
    int x;
    int y;
    char height;
    int shortest;
} Cell;

typedef struct QueueElement {
    Cell *cell;
    struct QueueElement *next;
} QueueElement;

void addNeighbourCell(PointerList *grid, int cx, int cy, int required_height, int required_score, QueueElement **tail) {
    Cell *cell = get_pointer(get_pointer(grid, cy), cx);

    if (cell != NULL && cell->height <= required_height && cell->shortest > required_score) {

        QueueElement *next = malloc(sizeof(QueueElement));

        cell->shortest = required_score;

        next->cell = cell;
        next->next = NULL;

        (*tail)->next = next;
        *tail = next;

    }
}

/*
 * Day 12, Part 1
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

    file = fopen("../days/day12/input_day12.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  total_divider, which will keep track of the smallest common denominator.
     */
    char line[200];

    PointerList *grid = initialize_pointerlist();

    int y = 0;

    Cell *start_cell;
    Cell *end_cell;

    /*
     * Loop over the input file.
     */
    while (fgets(line, 200, file) != NULL) {

        PointerList *current_line = initialize_pointerlist();
        add_pointer(grid, current_line);

        int x = 0;

        while (line[x] != '\n') {

            Cell *new_cell = malloc(sizeof(Cell));
            new_cell->x = x;
            new_cell->y = y;
            new_cell->shortest = 1000000000;
            new_cell->height = line[x];

            if (line[x] < '_') {

                if (line[x] == 'S') {

                    new_cell->shortest = 0;
                    new_cell->height = 'a';
                    start_cell = new_cell;

                } else if (line[x] == 'E') {

                    new_cell->height = 'z';
                    end_cell = new_cell;

                }
            }

            add_pointer(current_line, new_cell);
            x++;
        }

        y++;
    }

    QueueElement *head = malloc(sizeof(QueueElement));
    QueueElement *tail = head;

    head->cell = start_cell;
    head->next = NULL;

    while (1) {

        Cell *current = head->cell;

        int cx = current->x;
        int cy = current->y;

        if (cx == end_cell->x && cy == end_cell->y) {
            break;
        }

        int required_score = current->shortest + 1;
        int required_height = current->height + 1;

        addNeighbourCell(grid, cx, cy - 1, required_height, required_score, &tail);
        addNeighbourCell(grid, cx, cy + 1, required_height, required_score, &tail);
        addNeighbourCell(grid, cx - 1, cy, required_height, required_score, &tail);
        addNeighbourCell(grid, cx + 1, cy, required_height, required_score, &tail);

        QueueElement *temp = head;
        head = head->next;
        free(temp);
    }

    printf("%d\n", end_cell->shortest);

    while (head != NULL) {
        QueueElement *temp = head;
        head = head->next;
        free(temp);
    }

    for (int i = 0; i < grid->size; i++) {
        delete_pointerlist(get_pointer(grid, i));
    }

    delete_pointerlist_not_pointers(grid);

    fclose(file);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
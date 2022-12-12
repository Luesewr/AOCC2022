#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

/**
 * A type for each cell, containing its position, height and
 * the length of the shortest path to this cell from the start.
 */
typedef struct Cell {
    int x;
    int y;
    char height;
    int shortest;
} Cell;

/**
 * A linked list of cells for a queue.
 */
typedef struct QueueElement {
    Cell *cell;
    struct QueueElement *next;
} QueueElement;

/**
 * Checks if a neighbour at position (cx; cy) is a valid next position and adds it to the queue if so.
 * @param grid              A 2 dimensional list containing all cells.
 * @param cx                The horizontal position of the neighbour.
 * @param cy                The vertical position of the neighbour.
 * @param required_height   The maximum height allowed for the neighbour.
 * @param required_score    The score the neighbour will get.
 * @param tail              A pointer to the tail of the queue.
 */
void addNeighbourCell(PointerList *grid, int cx, int cy, int required_height, int required_score, QueueElement **tail) {

    Cell *cell = get_pointer(get_pointer(grid, cy), cx);

    if (cell != NULL && cell->height >= required_height && cell->shortest > 9999999) {

        QueueElement *next = malloc(sizeof(QueueElement));

        cell->shortest = required_score;

        next->cell = cell;
        next->next = NULL;

        (*tail)->next = next;
        *tail = next;
    }
}

/*
 * Day 12, Part 2
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
     *  grid, a 2-dimensional list containing all the cells in the input.
     *  y, to keep track of the row in the input.
     *  end_cell, to store the cell where we start.
     */
    char line[200];

    PointerList *grid = initialize_pointerlist();

    int y = 0;

    Cell *end_cell;

    /*
     * Loop over the input file.
     */
    while (fgets(line, 200, file) != NULL) {

        /*
         * Create a list to go in the grid for the current line of the input.
         */
        PointerList *current_line = initialize_pointerlist();
        add_pointer(grid, current_line);

        /*
         * Create variable x to keep track of the horizontal position of the cells to be added.
         */
        int x = 0;

        /*
         * Loop over the input line.
         */
        while (line[x] != '\n') {

            /*
             * Create a cell for the position.
             */
            Cell *new_cell = malloc(sizeof(Cell));

            new_cell->x = x;
            new_cell->y = y;
            new_cell->shortest = 1000000000;

            /*
             * Set special values if the cell is either the start or end,
             * else only set the height of the cell to the character.
             */
            if (line[x] < '_') {

                if (line[x] == 'S') {

                    new_cell->height = 'a';

                } else if (line[x] == 'E') {

                    new_cell->height = 'z';
                    new_cell->shortest = 0;
                    end_cell = new_cell;

                }
            } else {

                new_cell->height = line[x];

            }

            /*
             * Add the row to the grid.
             */
            add_pointer(current_line, new_cell);
            x++;
        }

        y++;
    }

    /*
     * Create a queue by setting a head and tail to the same queue element of the starting cell.
     */
    QueueElement *head = malloc(sizeof(QueueElement));
    QueueElement *tail = head;

    head->cell = end_cell;
    head->next = NULL;

    /*
     * Go through the queue of cells until the end cell is reached.
     */
    while (1) {

        /*
         * Get the cell at the front of the queue.
         */
        Cell *current = head->cell;

        int cx = current->x;
        int cy = current->y;

        /*
         * Check if we've reached the end.
         */
        if (current->height == 'a') {
            break;
        }

        /*
         * Go through all the neighbours of the current cell, update them and add them to the queue.
         */
        int required_score = current->shortest + 1;
        int required_height = current->height - 1;

        addNeighbourCell(grid, cx, cy - 1, required_height, required_score, &tail);
        addNeighbourCell(grid, cx, cy + 1, required_height, required_score, &tail);
        addNeighbourCell(grid, cx - 1, cy, required_height, required_score, &tail);
        addNeighbourCell(grid, cx + 1, cy, required_height, required_score, &tail);

        /*
         * Free the current queue element and go to the next element in the queue.
         */
        QueueElement *temp = head;
        head = head->next;
        free(temp);

    }

    /*
     * Print the length of the shortest path to the end cell.
     */
    printf("%d\n", head->cell->shortest);

    /*
     * Clean up all the allocated memory.
     */
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"

//816 too high
//815 too high

typedef struct Blizzard {
    int x;
    int y;
} Blizzard;

typedef struct point {
    int x;
    int y;
} point;

typedef struct QueueElement {
    int x;
    int y;
    int time;
    int has_reached_end;
    int has_reached_start;
    struct QueueElement *next;
} QueueElement;

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

/*
 * Day 24, Part 2
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

    file = fopen("../days/day24/input_day24.txt", "r");

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
    char line[125];

    PointerList *up = initialize_pointerlist();
    PointerList *down = initialize_pointerlist();
    PointerList *left = initialize_pointerlist();
    PointerList *right = initialize_pointerlist();

    fgets(line, 125, file);

    int x, y;

    int horizontal_size, vertical_size;

    y = 0;

    while (fgets(line, 125, file) != NULL) {
        if (line[1] == '#') {
            break;
        }

        x = 0;

        while (line[x + 1] != '#') {
            if (line[x + 1] == '.') {
                x++;
                continue;
            }

            Blizzard *new_blizzard = malloc(sizeof(Blizzard));
            new_blizzard->x = x;
            new_blizzard->y = y;

            PointerList *list;

            switch (line[x + 1]) {
                case '^':
                    list = up;
                    break;
                case 'v':
                    list = down;
                    break;
                case '<':
                    list = left;
                    break;
                case '>':
                    list = right;
                    break;
            }

            add_pointer(list, new_blizzard);

            x++;
        }

        horizontal_size = x;

        y++;
    }

    vertical_size = y;

    QueueElement *head = malloc(sizeof(QueueElement));
    QueueElement head_values = {0, -1, 0, 0, 0, NULL};
    *head = head_values;

    QueueElement *tail = head;

    int last_time = -1;

    PointerList *visited = initialize_pointerlist();

    int time_needed = -1;

    while (head != NULL) {
        QueueElement *current = head;

        if (current->time != last_time) {
            last_time = current->time;

            for (int i = 0; i < visited->size; i++) {
                point *current_point = get_pointer(visited, i);
                free(current_point);
            }

            visited->size = 0;
        }

        int is_duplicate = 0;

        for (int i = 0; i < visited->size; i++) {
            point *current_point = get_pointer(visited, i);

            if (current_point->x == current->x && current_point->y == current->y) {
                is_duplicate = 1;
                break;
            }
        }

        if (is_duplicate) {
            head = head->next;
            free(current);
            continue;
        }

//        printf("%d, %d, time: %d\n", current->x, current->y, current->time);

        int next_time = current->time + 1;

        if (current->has_reached_start && current->has_reached_end && current->y == vertical_size - 1 && current->x == horizontal_size - 1) {
            time_needed = current->time + 1;
            break;
        }

        if (current->has_reached_end && !current->has_reached_start && current->y == 0 && current->x == 0) {
            QueueElement *current_removal_element = head->next;
            while (current_removal_element != NULL) {
                QueueElement *temp = current_removal_element->next;
                free(current_removal_element);
                current_removal_element = temp;
            }

            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x, current->y - 1, next_time, 1, 1, NULL};
            *new_element = new_element_values;

            free(head);

            head = new_element;
            tail = new_element;

            continue;
        }

        if (!current->has_reached_end && !current->has_reached_start && current->y == vertical_size - 1 && current->x == horizontal_size - 1) {
            QueueElement *current_removal_element = head->next;
            while (current_removal_element != NULL) {
                QueueElement *temp = current_removal_element->next;
                free(current_removal_element);
                current_removal_element = temp;
            }

            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x, current->y + 1, next_time, 1, 0, NULL};
            *new_element = new_element_values;

            free(head);

            head = new_element;
            tail = new_element;

            continue;
        }

        int option_up = 1, option_down = 1, option_left = 1, option_right = 1;

        int can_stay = 1;

        for (int i = 0; i < up->size; i++) {
            Blizzard *current_blizzard = get_pointer(up, i);
            int current_blizzard_x = current_blizzard->x;
            int current_blizzard_y = mod(current_blizzard->y - next_time, vertical_size);

            if (current_blizzard_x == current->x && current_blizzard_y == current->y) {
                can_stay = 0;
            }

            if (manhattan_distance(current_blizzard_x, current_blizzard_y, current->x, current->y) == 1) {
                if (current_blizzard_y - current->y > 0) {
                    option_down = 0;
                } else if (current_blizzard_y - current->y < 0) {
                    option_up = 0;
                } else if (current_blizzard_x - current->x < 0) {
                    option_left = 0;
                } else if (current_blizzard_x - current->x > 0) {
                    option_right = 0;
                }
            }
        }

        for (int i = 0; i < down->size; i++) {
            Blizzard *current_blizzard = get_pointer(down, i);
            int current_blizzard_x = current_blizzard->x;
            int current_blizzard_y = mod(current_blizzard->y + next_time, vertical_size);

            if (current_blizzard_x == current->x && current_blizzard_y == current->y) {
                can_stay = 0;
            }

            if (manhattan_distance(current_blizzard_x, current_blizzard_y, current->x, current->y) == 1) {
                if (current_blizzard_y - current->y > 0) {
                    option_down = 0;
                } else if (current_blizzard_y - current->y < 0) {
                    option_up = 0;
                } else if (current_blizzard_x - current->x < 0) {
                    option_left = 0;
                } else if (current_blizzard_x - current->x > 0) {
                    option_right = 0;
                }
            }
        }

        for (int i = 0; i < left->size; i++) {
            Blizzard *current_blizzard = get_pointer(left, i);
            int current_blizzard_x = mod(current_blizzard->x - next_time, horizontal_size);
            int current_blizzard_y = current_blizzard->y;

            if (current_blizzard_x == current->x && current_blizzard_y == current->y) {
                can_stay = 0;
            }

            if (manhattan_distance(current_blizzard_x, current_blizzard_y, current->x, current->y) == 1) {
                if (current_blizzard_y - current->y > 0) {
                    option_down = 0;
                } else if (current_blizzard_y - current->y < 0) {
                    option_up = 0;
                } else if (current_blizzard_x - current->x < 0) {
                    option_left = 0;
                } else if (current_blizzard_x - current->x > 0) {
                    option_right = 0;
                }
            }
        }

        for (int i = 0; i < right->size; i++) {
            Blizzard *current_blizzard = get_pointer(right, i);
            int current_blizzard_x = mod(current_blizzard->x + next_time, horizontal_size);
            int current_blizzard_y = current_blizzard->y;

            if (current_blizzard_x == current->x && current_blizzard_y == current->y) {
                can_stay = 0;
            }

            if (manhattan_distance(current_blizzard_x, current_blizzard_y, current->x, current->y) == 1) {
                if (current_blizzard_y - current->y > 0) {
                    option_down = 0;
                } else if (current_blizzard_y - current->y < 0) {
                    option_up = 0;
                } else if (current_blizzard_x - current->x < 0) {
                    option_left = 0;
                } else if (current_blizzard_x - current->x > 0) {
                    option_right = 0;
                }
            }
        }

        if (can_stay) {
            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x, current->y, next_time, current->has_reached_end, current->has_reached_start, NULL};
            *new_element = new_element_values;

            tail->next = new_element;
            tail = new_element;
        }

        if (option_up && current->y - 1 >= 0) {
            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x, current->y - 1, next_time, current->has_reached_end, current->has_reached_start, NULL};
            *new_element = new_element_values;

            tail->next = new_element;
            tail = new_element;
        }

        if (option_down && current->y + 1 < vertical_size) {
            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x, current->y + 1, next_time, current->has_reached_end, current->has_reached_start, NULL};
            *new_element = new_element_values;

            tail->next = new_element;
            tail = new_element;
        }

        if (option_left && current->x - 1 >= 0 && current->y >= 0 && current->y < vertical_size) {
            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x - 1, current->y, next_time, current->has_reached_end, current->has_reached_start, NULL};
            *new_element = new_element_values;

            tail->next = new_element;
            tail = new_element;
        }

        if (option_right && current->x + 1 < horizontal_size && current->y >= 0 && current->y < vertical_size) {
            QueueElement *new_element = malloc(sizeof(QueueElement));
            QueueElement new_element_values = {current->x + 1, current->y, next_time, current->has_reached_end, current->has_reached_start, NULL};
            *new_element = new_element_values;

            tail->next = new_element;
            tail = new_element;
        }

        point *new_point = malloc(sizeof(point));
        new_point->x = current->x;
        new_point->y = current->y;
        add_pointer(visited, new_point);

        head = head->next;
        free(current);
    }

    printf("%d\n", time_needed);

    QueueElement *current_removal_element = head;
    while (current_removal_element != NULL) {
        QueueElement *temp = current_removal_element->next;
        free(current_removal_element);
        current_removal_element = temp;
    }

    delete_pointerlist(up);
    delete_pointerlist(down);
    delete_pointerlist(left);
    delete_pointerlist(right);
    delete_pointerlist(visited);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
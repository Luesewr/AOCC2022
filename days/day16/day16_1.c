#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day16_functions.h"

#define MINUTES 30

/*
 * Day 16, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    PointerList *names = initialize_pointerlist();
    PointerList *nodes = initialize_pointerlist();
    PointerList *edge_names = initialize_pointerlist();

    int start_index = parse_input(names, nodes, edge_names);

    link_outgoing_edges(names, nodes, edge_names);

    int ** distances = calculate_minimum_distances(nodes);


    int nodes_size = nodes->size;


    QueueElement *head = malloc(sizeof(QueueElement));
    QueueElement head_values = {start_index, 0, MINUTES, 0, NULL};
    *head = head_values;
    QueueElement *tail = head;

    for (int i = 0; i < nodes_size; i++) {
        if (((Valve *) get_pointer(nodes, i))->flow_rate == 0) {
            int64_t mask = 1;
            head->visited_nodes |= mask << i;
        }
    }

    int maximum_pressure = 0;

    while (head != NULL) {
        QueueElement *current_element = head;
        int64_t current_visited = current_element->visited_nodes;
        Valve *current_node = get_pointer(nodes, current_element->node_id);

        int new_total = current_element->total + current_node->flow_rate * current_element->minutes_left;
        current_element->total = new_total;

        int could_add = 0;

        for (int i = 0; i < nodes_size; i++) {
            int new_minutes_left = current_element->minutes_left - distances[current_element->node_id][i] - 1;
            if (!bitAt(current_visited, i) && new_minutes_left >= 0) {
                could_add = 1;
                QueueElement *new_element = malloc(sizeof(QueueElement));
                QueueElement new_element_values = {i, new_total, new_minutes_left, setBitOneAt(current_visited, i), NULL};
                *new_element = new_element_values;
                tail->next = new_element;
                tail = tail->next;
            }
        }

        if (!could_add && new_total > maximum_pressure) {
            maximum_pressure = new_total;
        }

        head = head->next;
        free(current_element);
    }

    printf("%d\n", maximum_pressure);

    for (int i = 0; i < nodes_size; i++) {
        delete_pointerlist(((Valve *) get_pointer(nodes, i))->edges);
        free(distances[i]);
    }

    free(distances);

    delete_pointerlist(nodes);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

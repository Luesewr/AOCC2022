#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day16_functions.h"

#define MINUTES 26

/*
 * Day 16, Part 2
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

    ElephantQueueElement *head = malloc(sizeof(ElephantQueueElement));
    ElephantQueueElement head_values = {start_index, start_index, 0, MINUTES, MINUTES, 0, PLAYER, NULL};
    *head = head_values;
    ElephantQueueElement *tail = head;

    for (int i = 0; i < nodes_size; i++) {
        if (((Valve *) get_pointer(nodes, i))->flow_rate == 0) {
            int64_t mask = 1;
            head->visited_nodes |= mask << i;
        }
    }

    int maximum_pressure = 0;

    while (head != NULL) {
        ElephantQueueElement *current_element = head;
        int64_t current_visited = current_element->visited_nodes;

        if (current_element->type == PLAYER) {
            Valve *current_node = get_pointer(nodes, current_element->node_id);
            current_element->total += current_node->flow_rate * current_element->minutes_left;
        } else {
            Valve *current_elephant_node = get_pointer(nodes, current_element->elephant_node_id);
            current_element->total += current_elephant_node->flow_rate * current_element->elephant_minutes_left;
        }

        int could_add = 0;
        for (int i = 0; i < nodes_size; i++) {
            if (current_element->minutes_left > current_element->elephant_minutes_left) {
                int new_minutes = current_element->minutes_left - distances[current_element->node_id][i] - 1;
                if (!bitAt(current_visited, i) && new_minutes >= 0) {
                    could_add = 1;
                    ElephantQueueElement *new_element = malloc(sizeof(ElephantQueueElement));
                    ElephantQueueElement new_element_values = {i,
                                                               current_element->elephant_node_id,
                                                               current_element->total,
                                                               current_element->elephant_minutes_left,
                                                               new_minutes,
                                                               setBitOneAt(current_visited, i),
                                                               PLAYER,
                                                               NULL};
                    *new_element = new_element_values;
                    tail->next = new_element;
                    tail = tail->next;
                }
            }
            if (current_element->minutes_left <= current_element->elephant_minutes_left) {
                int new_minutes = current_element->elephant_minutes_left - distances[current_element->elephant_node_id][i] - 1;
                if (!bitAt(current_visited, i) && new_minutes >= 0) {
                    could_add = 1;
                    ElephantQueueElement *new_element = malloc(sizeof(ElephantQueueElement));
                    ElephantQueueElement new_element_values = {current_element->node_id,
                            i,
                            current_element->total,
                            new_minutes,
                            current_element->minutes_left,
                            setBitOneAt(current_visited, i),
                            ELEPHANT,
                            NULL};
                    *new_element = new_element_values;
                    tail->next = new_element;
                    tail = tail->next;
                }
            }
        }

        if (!could_add && current_element->total > maximum_pressure) {
            maximum_pressure = current_element->total;
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

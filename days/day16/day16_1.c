#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day16_functions.h"

#define MINUTES 30

PointerList *current_nodes;

int sort_by_flow_rate(int i, int j) {
    Valve *node_i = get_pointer(current_nodes, i);
    Valve *node_j = get_pointer(current_nodes, j);
    int flow_rate_i = node_i->flow_rate;
    int flow_rate_j = node_j->flow_rate;
    return flow_rate_j - flow_rate_i;
}

/*
 * Day 16, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Create lists to store the graph information.
     */
    PointerList *names = initialize_pointerlist();
    PointerList *nodes = initialize_pointerlist();
    PointerList *edge_names = initialize_pointerlist();

    /*
     * Parse the input into the lists and get the starting index.
     */
    int start_index = parse_input(names, nodes, edge_names);

    /*
     * Link the edges of each node to the right index in the nodes list.
     */
    link_outgoing_edges(names, nodes, edge_names);

    /*
     * Calculate the minimum distance to get to each node from each node.
     */
    int ** distances = calculate_minimum_distances(nodes);

    /*
     * Store the amount of nodes, because it is used a lot.
     */
    int nodes_size = nodes->size;

    current_nodes = nodes;

    PointerList *traversal_order = initialize_pointerlist_of_capacity(nodes_size);

    for (int j = 0; j < nodes_size; j++) {
        add_int(traversal_order, j);
    }

    sort_ints_with_comparator(traversal_order, &sort_by_flow_rate);

    /*
     * Create the tail of the stack, starting at the starting index with 30 minutes left.
     */
    StackElement *tail = malloc(sizeof(StackElement));
    StackElement tail_values = {start_index, 0, MINUTES, 0, NULL};
    *tail = tail_values;

    /*
     * Set the nodes with a flow rate of 0 to be visited already.
     */
    for (int i = 0; i < nodes_size; i++) {

        if (((Valve *) get_pointer(nodes, i))->flow_rate == 0) {
            int64_t mask = 1;
            tail->visited_nodes |= mask << i;
        }
    }

    /*
     * Create a variable to keep track of the maximum pressure.
     */
    int maximum_pressure = 0;

    /*
     * Loop as long as the stack is not empty.
     */
    while (tail != NULL) {

        /*
         * Get the last element in the stack, its corresponding visited nodes and the current node, then update the tail.
         */
        StackElement *current_element = tail;
        tail = tail->prev;
        int64_t current_visited = current_element->visited_nodes;
        Valve *current_node = get_pointer(nodes, current_element->node_id);

        /*
         * Update the total of the current element.
         */
        current_element->total += current_node->flow_rate * current_element->minutes_left;

        /*
         * Add variable to check if this is the end of this possible order of nodes.
         */
        int could_add = 0;

        int maximum_additional_pressure = 0;

        int minutes_left = current_element->minutes_left - 2;

        for (int i = 0; i < nodes_size && minutes_left >= 0; i++) {
            int current_node_index = get_int(traversal_order, i);
            Valve *node_at_index = get_pointer(nodes, current_node_index);

            if (bitAt(current_visited, current_node_index)) {
                continue;
            }

            maximum_additional_pressure += node_at_index->flow_rate * minutes_left;

            minutes_left -= 2;
        }

        if (maximum_additional_pressure + current_element->total <= maximum_pressure) {
            free(current_element);
            continue;
        }

        /*
         * Go through all the nodes.
         */
        for (int i = nodes_size - 1; i >= 0; i--) {
            int node_index = get_int(traversal_order, i);

            /*
             * Check if this node is already visited and if the time that it would give it
             * would still have 0 or more minutes remaining.
             */
            int new_minutes_left;

            if (    (!bitAt(current_visited, node_index)) &&
                    (new_minutes_left = current_element->minutes_left - distances[current_element->node_id][node_index] - 1) &&
                    (new_minutes_left >= 0)) {
                /*
                 * Mark that there was a new node available and add that node to the stack with the new calculated remaining time.
                 */
                could_add = 1;

                StackElement *new_element = malloc(sizeof(StackElement));
                StackElement new_element_values = {node_index, current_element->total, new_minutes_left, setBitOneAt(current_visited, node_index), tail};
                *new_element = new_element_values;

                tail = new_element;
            }
        }

        /*
         * If this node was the last of this possibility of nodes, update the maximum pressure
         * if this current pressure is bigger.
         */
        if (!could_add && current_element->total > maximum_pressure) {
            maximum_pressure = current_element->total;
        }

        free(current_element);
    }

    /*
     * Print the maximum pressure.
     */
    printf("%d\n", maximum_pressure);

    /*
     * Clean up the allocated memory.
     */
    for (int i = 0; i < nodes_size; i++) {
        delete_pointerlist(((Valve *) get_pointer(nodes, i))->edges);
        free(distances[i]);
    }

    free(distances);

    delete_pointerlist(nodes);

    delete_pointerlist(traversal_order);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

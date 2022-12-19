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

    /*
     * Create the tail of the stack, both starting at the starting index with 26 minutes left.
     */
    ElephantStackElement *tail = malloc(sizeof(ElephantStackElement));
    ElephantStackElement tail_values = {start_index, start_index, 0, MINUTES, MINUTES, 0, PLAYER, NULL};
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
        ElephantStackElement *current_element = tail;

        tail = tail->prev;
        int64_t current_visited = current_element->visited_nodes;

        /*
         * Update the total of the current element according to whether this is currently
         * the elephant or the person visiting.
         */
        if (current_element->type == PLAYER) {
            Valve *current_node = get_pointer(nodes, current_element->node_id);
            current_element->total += current_node->flow_rate * current_element->minutes_left;
        } else {
            Valve *current_elephant_node = get_pointer(nodes, current_element->elephant_node_id);
            current_element->total += current_elephant_node->flow_rate * current_element->elephant_minutes_left;
        }

        /*
         * Add variable to check if this is the end of this possible order of nodes.
         */
        int could_add = 0;

        /*
         * Check if it's the elephant's or the person's turn.
         */
        if (current_element->minutes_left > current_element->elephant_minutes_left) {
            /*
             * Go through all the nodes.
             */
            for (int i = 0; i < nodes_size; i++) {

                /*
                 * Check if this node is already visited and if the time that it would give it
                 * would still have 0 or more minutes remaining.
                 */
                int new_minutes;

                if (    (!bitAt(current_visited, i)) &&
                        (new_minutes  = current_element->minutes_left - distances[current_element->node_id][i] - 1) &&
                        (new_minutes >= 0)) {
                    /*
                     * Mark that there was a new node available and add that node to the stack with the new calculated remaining time.
                     */
                    could_add = 1;

                    ElephantStackElement *new_element = malloc(sizeof(ElephantStackElement));
                    ElephantStackElement new_element_values = {i,
                                                               current_element->elephant_node_id,
                                                               current_element->total,
                                                               current_element->elephant_minutes_left,
                                                               new_minutes,
                                                               setBitOneAt(current_visited, i),
                                                               PLAYER,
                                                               tail};
                    *new_element = new_element_values;

                    tail = new_element;
                }
            }
        } else {
            /*
             * Go through all the nodes.
             */
            for (int i = 0; i < nodes_size; i++) {

                /*
                 * Check if this node is already visited and if the time that it would give it
                 * would still have 0 or more minutes remaining.
                 */
                int new_minutes;

                if (    (!bitAt(current_visited, i)) &&
                        (new_minutes = current_element->elephant_minutes_left - distances[current_element->elephant_node_id][i] - 1) &&
                        (new_minutes >= 0)) {
                    /*
                     * Mark that there was a new node available and add that node to the stack with the new calculated remaining time.
                     */
                    could_add = 1;

                    ElephantStackElement *new_element = malloc(sizeof(ElephantStackElement));
                    ElephantStackElement new_element_values = {current_element->node_id,
                                                               i,
                                                               current_element->total,
                                                               new_minutes,
                                                               current_element->minutes_left,
                                                               setBitOneAt(current_visited, i),
                                                               ELEPHANT,
                                                               tail};
                    *new_element = new_element_values;

                    tail = new_element;
                }
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

    printf("%d\n", maximum_pressure);

    for (int i = 0; i < nodes_size; i++) {
        delete_pointerlist(((Valve *) get_pointer(nodes, i))->edges);
        free(distances[i]);
    }

    /*
     * Print the maximum pressure.
     */
    free(distances);

    /*
     * Clean up the allocated memory.
     */
    delete_pointerlist(nodes);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

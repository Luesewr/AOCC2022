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
     * Create the head and tail of the queue, starting at the starting index with 30 minutes left.
     */
    QueueElement *head = malloc(sizeof(QueueElement));
    QueueElement head_values = {start_index, 0, MINUTES, 0, NULL};
    *head = head_values;
    QueueElement *tail = head;

    /*
     * Set the nodes with a flow rate of 0 to be visited already.
     */
    for (int i = 0; i < nodes_size; i++) {

        if (((Valve *) get_pointer(nodes, i))->flow_rate == 0) {
            int64_t mask = 1;
            head->visited_nodes |= mask << i;
        }
    }

    /*
     * Create a variable to keep track of the maximum pressure.
     */
    int maximum_pressure = 0;

    /*
     * Loop as long as the queue is not empty.
     */
    while (head != NULL) {

        /*
         * Get the current element in the queue, its corresponding visited nodes and the current node.
         */
        QueueElement *current_element = head;
        int64_t current_visited = current_element->visited_nodes;
        Valve *current_node = get_pointer(nodes, current_element->node_id);

        /*
         * Update the total of the current element in the queue.
         */
        current_element->total += current_node->flow_rate * current_element->minutes_left;

        /*
         * Add variable to check if this is the end of this possible order of nodes.
         */
        int could_add = 0;

        /*
         * Go through all the nodes.
         */
        for (int i = 0; i < nodes_size; i++) {

            /*
             * Check if this node is already visited and if the time that it would give it
             * would still have 0 or more minutes remaining.
             */
            int new_minutes_left;

            if (    (!bitAt(current_visited, i)) &&
                    (new_minutes_left = current_element->minutes_left - distances[current_element->node_id][i] - 1) &&
                    (new_minutes_left >= 0)) {
                /*
                 * Mark that there was a new node available and add that node to the queue with the new calculated remaining time.
                 */
                could_add = 1;

                QueueElement *new_element = malloc(sizeof(QueueElement));
                QueueElement new_element_values = {i, current_element->total, new_minutes_left, setBitOneAt(current_visited, i), NULL};
                *new_element = new_element_values;

                tail->next = new_element;
                tail = tail->next;
            }
        }

        /*
         * If this node was the last of this possibility of nodes, update the maximum pressure
         * if this current pressure is bigger.
         */
        if (!could_add && current_element->total > maximum_pressure) {
            maximum_pressure = current_element->total;
        }

        /*
         * Go to the next node in the queue and free the current node.
         */
        head = head->next;
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

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

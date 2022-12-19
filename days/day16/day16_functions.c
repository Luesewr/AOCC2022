#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../library/pointerList.h"
#include "day16_functions.h"
#include "../../library/intList.h"

/**
 * Reset all the values in the array.
 * @param array     An array of integers.
 * @param size      The size of the array.
 */
void reset_visited(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

/**
 * Returns the value of the bit at an index in an integer.
 * @param list      A 64-bit integer containing all the visited node statuses.
 * @param index     The index of the bit we're retrieving.
 * @return      The value of the retrieved bit.
 */
int bitAt(int64_t list, int index) {
    int64_t mask = 1;
    return (int) ((list >> index) & mask);
}

/**
 * Sets the value of a bit at an index in an integer to 1.
 * @param list      A 64-bit integer containing all the visited node statuses.
 * @param index     The index of the bit we're setting.
 * @return      The value of the list after it has been updated.
 */
int64_t setBitOneAt(int64_t list, int index) {
    int64_t mask = 1;
    return list | (mask << index);
}

/**
 * Parses the input file into the lists given.
 * @param names         A list that contains all of the names of the nodes.
 * @param nodes         A list containing all of the node objects.
 * @param edge_names    A list of lists of the names of the nodes that a node is attached to.
 * @return          The index of the starting node.
 */
int parse_input(PointerList *names, PointerList *nodes, PointerList *edge_names) {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day16/input_day16.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        exit(1);
    }

    /*
     * Create variables:
     *  line, to store the input line.
     *  id, to keep track of the id of the node that we're currently creating.
     *  start_index, which keeps track of the index with a node with the name "AA".
     */
    char line[75];
    int id = 0;
    int start_index;

    /*
     * Loop over the entire input.
     */
    while (fgets(line, 75, file) != NULL) {

        /*
         * Create a node.
         */
        Valve *current_node = malloc(sizeof(Valve));

        add_pointer(nodes, current_node);

        /*
         * Add the name of the node to the names list.
         */
        char *ptr = line + 6;

        char *name = malloc(sizeof(char) * 3);
        name[0] = ptr[0];
        name[1] = ptr[1];
        name[2] = '\0';
        add_pointer(names, name);

        /*
         * If the name of the current node is "AA" then set the starting index to the current id.
         */
        if (strcmp(name, "AA") == 0) {
            start_index = id;
        }

        id++;


        ptr += 17;

        /*
         * Parse the flow rate of the node.
         */
        current_node->flow_rate = (int) strtol(ptr, &ptr, 10);

        /*
         * Move the pointer correctly regardless of singular of multiple neighbour nodes.
         */
        ptr += (ptr[8] == 's') + 22;

        PointerList *outgoing_edges = initialize_pointerlist();

        /*
         * Loop through the neighbour nodes until we reach the end of the line.
         */
        while (ptr[0] != '\n' && ptr[0] != '\r') {
            ptr += 2;

            /*
             * Get the name of the neighbour and add it to the outgoing edges list.
             */
            char *edge = malloc(sizeof(char) * 3);
            edge[0] = ptr[0];
            edge[1] = ptr[1];
            edge[2] = '\0';

            add_pointer(outgoing_edges, edge);
            ptr += 2;
        }

        add_pointer(edge_names, outgoing_edges);
    }

    fclose(file);

    return start_index;
}

/**
 * Links together the nodes by setting the correct values of the indices in the neighbour list of each node.
 * @param names         A list containing all the names of all the nodes.
 * @param nodes         A list containing all of the node objects.
 * @param edge_names    A list of lists containing all the names of the neighbours of each node.
 */
void link_outgoing_edges(PointerList *names, PointerList *nodes, PointerList *edge_names) {
    int nodes_size = nodes->size;

    /*
     * Loop through all the nodes.
     */
    for (int i = 0; i < nodes_size; i++) {

        /*
         * Get the current values.
         */
        Valve *current_node = get_pointer(nodes, i);

        PointerList *current_names = get_pointer(edge_names, i);

        /*
         * Create the list of edges for the node.
         */
        PointerList *edges = initialize_pointerlist_of_capacity(current_names->size);
        current_node->edges = edges;

        /*
         * Go through all names in the edge names.
         */
        for (int j = 0; j < current_names->size; j++) {

            /*
             * Go through all the names of the nodes to see which one matches.
             */
            for (int k = 0; k < names->size; k++) {

                char * node_name = get_pointer(names, k);
                char * name = get_pointer(current_names, j);

                /*
                 * If the name matches add the index of this node to the edge list of the current node.
                 */
                if (strcmp(node_name, name) == 0) {
                    add_int(edges, k);
                    break;
                }
            }
        }

        delete_pointerlist(current_names);
    }

    delete_pointerlist_not_pointers(edge_names);

    delete_pointerlist(names);
}

/**
 * Calculates the minimum distance from each node to each node using BFS.
 * @param nodes
 * @return
 */
int **calculate_minimum_distances(PointerList *nodes) {

    int nodes_size = nodes->size;

    /*
     * Create a 2-dimensional integer array to store all the distances.
     */
    int ** distances = malloc(sizeof(int *) * nodes_size);

    /*
     * Loop through all the nodes.
     */
    for (int i = 0; i < nodes_size; i++) {

        /*
         * Create an integer array to store the distances of the current node.
         */
        distances[i] = malloc(sizeof(int) * nodes_size);

        /*
         * Create an array to store all the nodes that have been visited already.
         */
        int visited[nodes_size];
        reset_visited(visited, nodes_size);
        visited[i] = 1;

        /*
         * Create the starting element of the queue.
         */
        ValveQueueElement *head = malloc(sizeof(ValveQueueElement));
        head->node_id = i;
        head->total = 0;
        head->next = NULL;
        ValveQueueElement *tail = head;

        /*
         * Loop as long as the queue isn't empty.
         */
        while (head != NULL) {

            /*
             * Get the current element.
             */
            ValveQueueElement *current = head;

            /*
             * Set the distance value of this node to the depth of this node.
             */
            distances[i][current->node_id] = current->total;

            /*
             * Get the edges of the current valve.
             */
            PointerList *edges = ((Valve *) get_pointer(nodes, current->node_id))->edges;

            /*
             * Go through all the edges.
             */
            for (int j = 0; j < edges->size; j++) {

                /*
                 * Get the current edge index.
                 */
                int current_edge = get_int(edges, j);

                /*
                 * If we haven't visited it before add the node to the queue.
                 */
                if (visited[current_edge] == 0) {
                    ValveQueueElement *new_element = malloc(sizeof(ValveQueueElement));

                    new_element->node_id = current_edge;
                    new_element->total = current->total + 1;
                    new_element->next = NULL;

                    visited[current_edge] = 1;

                    tail->next = new_element;
                    tail = tail->next;
                }
            }

            /*
             * Go to the next element in the queue.
             */
            head = head->next;
            free(current);
        }
    }

    return distances;
}
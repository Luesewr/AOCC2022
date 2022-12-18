#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../library/pointerList.h"
#include "day16_functions.h"
#include "../../library/intList.h"

void reset_visited(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}
int bitAt(int64_t list, int index) {
    int64_t mask = 1;
    return (int) ((list >> index) & mask);
}

int64_t setBitOneAt(int64_t list, int index) {
    int64_t mask = 1;
    return list | (mask << index);
}

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

    char line[75];
    int id = 0;
    int start_index;
    /*
     * Loop over the entire input.
     */

    while (fgets(line, 75, file) != NULL) {
        Valve *current_node = malloc(sizeof(Valve));

        add_pointer(nodes, current_node);

        char *ptr = line + 6;
        char *name = malloc(sizeof(char) * 3);
        name[0] = ptr[0];
        name[1] = ptr[1];
        name[2] = '\0';
        add_pointer(names, name);

        if (strcmp(name, "AA") == 0) {
            start_index = id;
        }
        id++;

        ptr += 17;

        current_node->flow_rate = (int) strtol(ptr, &ptr, 10);

        ptr += (ptr[8] == 's') + 22;

        PointerList *outgoing_edges = initialize_pointerlist();

        while (ptr[0] != '\n' && ptr[0] != '\r') {
            ptr += 2;
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

void link_outgoing_edges(PointerList *names, PointerList *nodes, PointerList *edge_names) {
    int nodes_size = nodes->size;

    for (int i = 0; i < nodes_size; i++) {
        Valve *current_node = get_pointer(nodes, i);

        PointerList *current_names = get_pointer(edge_names, i);

        PointerList *edges = initialize_pointerlist_of_capacity(current_names->size);
        current_node->edges = edges;

        for (int j = 0; j < current_names->size; j++) {
            for (int k = 0; k < names->size; k++) {
                char * node_name = get_pointer(names, k);
                char * name = get_pointer(current_names, j);

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

int **calculate_minimum_distances(PointerList *nodes) {
    int nodes_size = nodes->size;

    int ** distances = malloc(sizeof(int *) * nodes_size);

    for (int i = 0; i < nodes_size; i++) {
        distances[i] = malloc(sizeof(int) * nodes_size);

        int visited[nodes_size];
        reset_visited(visited, nodes_size);
        visited[i] = 1;

        ValveQueueElement *head = malloc(sizeof(ValveQueueElement));
        head->node_id = i;
        head->total = 0;
        head->next = NULL;
        ValveQueueElement *tail = head;

        while (head != NULL) {
            ValveQueueElement *current = head;
            distances[i][current->node_id] = current->total;

            PointerList *edges = ((Valve *) get_pointer(nodes, current->node_id))->edges;
            for (int j = 0; j < edges->size; j++) {
                int current_edge = get_int(edges, j);
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

            head = head->next;
            free(current);
        }
    }

    return distances;
}
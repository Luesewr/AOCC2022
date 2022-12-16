#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define SECONDS 30

typedef struct Node {
    int id;
    int flow_rate;
    PointerList *edges;
    PointerList *best;
} Node;

typedef struct QueueElement {
    int node_id;
    int seconds_left;
    int nodes_activated;
    PointerList *activated_nodes;
    struct QueueElement *next;
} QueueElement;

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
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day16/testInput_day16.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    char line[75];

    PointerList *names = initialize_pointerlist();
    PointerList *nodes = initialize_pointerlist();
    PointerList *outgoing_edge_names = initialize_pointerlist();

    /*
     * Loop over the entire input.
     */
    int id = 0;

    while (fgets(line, 75, file) != NULL) {
        Node *current_node = malloc(sizeof(Node));

        add_pointer(nodes, current_node);

        current_node->id = id;
        id++;

        char *ptr = line + 6;
        char *name = malloc(sizeof(char) * 3);
        name[0] = ptr[0];
        name[1] = ptr[1];
        name[2] = '\0';
        add_pointer(names, name);

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

        add_pointer(outgoing_edge_names, outgoing_edges);

        PointerList *best = initialize_pointerlist_of_capacity(SECONDS);

        for (int i = 0; i < SECONDS; i++) {
            add_int(best, 0);
        }

        current_node->best = best;
    }

    for (int i = 0; i < nodes->size; i++) {
        Node *current_node = get_pointer(nodes, i);

        PointerList *current_names = get_pointer(outgoing_edge_names, i);

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

    delete_pointerlist_not_pointers(outgoing_edge_names);

    delete_pointerlist(names);

    print_ints(((Node *) get_pointer(nodes, 0))->edges);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

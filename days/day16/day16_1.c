#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define MINUTES 30

typedef struct Node {
    int flow_rate;
    PointerList *edges;
} Node;

typedef struct NodeQueueElement {
    int node_id;
    int total;
    struct NodeQueueElement *next;
} NodeQueueElement;

typedef struct QueueElement {
    int node_id;
    int total;
    int minutes_left;
    int *visited_nodes;
    struct QueueElement *next;
} QueueElement;

void reset_visited(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void copy_into_array(const int *source, int *dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
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
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day16/input_day16.txt", "r");

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
    int start_index;

    while (fgets(line, 75, file) != NULL) {
        Node *current_node = malloc(sizeof(Node));

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

        add_pointer(outgoing_edge_names, outgoing_edges);
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

    int ** distances = malloc(sizeof(int *) * nodes->size);

    for (int i = 0; i < nodes->size; i++) {
        distances[i] = malloc(sizeof(int) * nodes->size);

        int visited[nodes->size];
        reset_visited(visited, nodes->size);
        visited[i] = 1;

        NodeQueueElement *head = malloc(sizeof(NodeQueueElement));
        head->node_id = i;
        head->total = 0;
        head->next = NULL;
        NodeQueueElement *tail = head;

        while (head != NULL) {
            NodeQueueElement *current = head;
            distances[i][current->node_id] = current->total;

            PointerList *edges = ((Node *) get_pointer(nodes, current->node_id))->edges;
            for (int j = 0; j < edges->size; j++) {
                int current_edge = get_int(edges, j);
                if (visited[current_edge] == 0) {
                    NodeQueueElement *new_element = malloc(sizeof(NodeQueueElement));
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

    QueueElement *head = malloc(sizeof(QueueElement));
    head->node_id = start_index;
    head->minutes_left = MINUTES + 1;
    head->total = 0;
    head->next = NULL;
    head->visited_nodes = malloc(sizeof(int) * nodes->size);

    for (int i = 0; i < nodes->size; i++) {
        if (((Node *) get_pointer(nodes, i))->flow_rate == 0) {
            head->visited_nodes[i] = 1;
        } else {
            head->visited_nodes[i] = 0;
        }
    }

    QueueElement *tail = head;

    int maximum_pressure = 0;

    while (head != NULL) {
        QueueElement *current_element = head;
        current_element->minutes_left--;
        int *current_visited = current_element->visited_nodes;
        Node *current_node = get_pointer(nodes, current_element->node_id);
        current_element->total += current_node->flow_rate * current_element->minutes_left;
        int could_add = 0;
        for (int i = 0; i < nodes->size; i++) {
            int distance = distances[current_element->node_id][i];
            if (current_visited[i] == 0 && current_element->minutes_left - distance - 1 >= 0) {
                could_add = 1;
                QueueElement *new_element = malloc(sizeof(QueueElement));
                new_element->visited_nodes = malloc(sizeof(int) * nodes->size);
                copy_into_array(current_element->visited_nodes, new_element->visited_nodes, nodes->size);
                new_element->visited_nodes[i] = 1;
                new_element->minutes_left = current_element->minutes_left - distance;
                new_element->node_id = i;
                new_element->total = current_element->total;
                new_element->next = NULL;

                tail->next = new_element;
                tail = tail->next;
            }
        }
        if (!could_add && current_element->total > maximum_pressure) {
            maximum_pressure = current_element->total;
        }

        head = head->next;
        free(current_element->visited_nodes);
        free(current_element);
    }

    printf("%d\n", maximum_pressure);

    for (int i = 0; i < nodes->size; i++) {
        delete_pointerlist(((Node *) get_pointer(nodes, i))->edges);
        free(distances[i]);
    }

    free(distances);

    fclose(file);

    delete_pointerlist(nodes);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

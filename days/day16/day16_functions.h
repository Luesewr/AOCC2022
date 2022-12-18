//
// Created by Laure on 2022-12-18.
//

#ifndef POINTERLIST_DAY16_FUNCTIONS_H
#define POINTERLIST_DAY16_FUNCTIONS_H

#include "../../library/pointerList.h"
#include <stdint.h>

typedef struct Valve {
    int flow_rate;
    PointerList *edges;
} Valve;

typedef struct ValveQueueElement {
    int node_id;
    int total;
    struct ValveQueueElement *next;
} ValveQueueElement;

typedef struct QueueElement {
    int node_id;
    int total;
    int minutes_left;
    int64_t visited_nodes;
    struct QueueElement *next;
} QueueElement;

typedef enum{PLAYER, ELEPHANT} ElementFocus;

typedef struct ElephantQueueElement {
    int node_id;
    int elephant_node_id;
    int total;
    int elephant_minutes_left;
    int minutes_left;
    int64_t visited_nodes;
    ElementFocus type;
    struct ElephantQueueElement *next;
} ElephantQueueElement;

void reset_visited(int array[], int size);

int bitAt(int64_t list, int index);

int64_t setBitOneAt(int64_t list, int index);

QueueElement *create_queue_element(int node_id, int total, int minutes_left, int64_t visited_nodes);

ElephantQueueElement *create_elephant_queue_element(int node_id, int elephant_node_id, int total, int elephant_minutes_left, int minutes_left, int64_t visited_nodes, ElementFocus type);

int parse_input(PointerList *names, PointerList *nodes, PointerList *edge_names);

void link_outgoing_edges(PointerList *names, PointerList *nodes, PointerList *edge_names);

int **calculate_minimum_distances(PointerList *nodes);

#endif //POINTERLIST_DAY16_FUNCTIONS_H
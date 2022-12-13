#include <stdio.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"
#include "day13_functions.h"

/*
 * Day 13, Part 2
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    PointerList *packets = parseInput();

    PointerList *custom_packet_2 = initialize_pointerlist();
    PointerList *packet_2 = initialize_pointerlist();
    add_int(packet_2, 2);
    add_pointer(custom_packet_2, packet_2);

    PointerList *custom_packet_6 = initialize_pointerlist();
    PointerList *packet_6 = initialize_pointerlist();
    add_int(packet_6, 6);
    add_pointer(custom_packet_6, packet_6);

    add_pointer(packets, custom_packet_2);
    add_pointer(packets, custom_packet_6);

    sort_pointers_with_comparator(packets, &packetComparator);

    int decoder_key = (index_of(packets, custom_packet_2) + 1) * (index_of(packets, custom_packet_6) + 1);

    printf("%d\n", decoder_key);

    /*
     * Close the timer and print the taken time.
     */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
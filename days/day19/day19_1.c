#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define MINUTES 24

#define ROBOT_AMOUNT 4

typedef enum Material {
    ORE,
    CLAY,
    OBSIDIAN,
    GEODE
} Material;

typedef struct Robot {
    Material material;
    int prices[ROBOT_AMOUNT];
} Robot;

typedef struct Blueprint {
    int id;
    Robot robots[ROBOT_AMOUNT];
} Blueprint;

typedef struct StackElement {
    int minutes_left;
    int material_count[ROBOT_AMOUNT];
    int robot_count[ROBOT_AMOUNT];
} StackElement;

/*
 * Day 19, Part 1
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

    file = fopen("../days/day19/testInput_day19.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables:
     *  line, to store the input line.
     */
    char line[200];
    PointerList *blueprints = initialize_pointerlist();

    /*
     * Loop over the entire input.
     */
    while (fgets(line, 200, file) != NULL) {

        char *ptr = line + 10;

        Blueprint *current_blueprint = malloc(sizeof(Blueprint));

        add_pointer(blueprints, current_blueprint);

        current_blueprint->id = (int) strtol(ptr, &ptr, 10);


        ptr += 23;

        current_blueprint->robots[0].material = ORE;
        current_blueprint->robots[0].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[0].prices[1] = 0;
        current_blueprint->robots[0].prices[2] = 0;
        current_blueprint->robots[0].prices[3] = 0;


        ptr += 28;

        current_blueprint->robots[1].material = CLAY;
        current_blueprint->robots[1].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[1].prices[1] = 0;
        current_blueprint->robots[1].prices[2] = 0;
        current_blueprint->robots[0].prices[3] = 0;


        ptr += 32;

        current_blueprint->robots[2].material = OBSIDIAN;
        current_blueprint->robots[2].prices[0] = (int) strtol(ptr, &ptr, 10);

        ptr += 9;

        current_blueprint->robots[2].prices[1] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[2].prices[2] = 0;
        current_blueprint->robots[0].prices[3] = 0;


        ptr += 30;

        current_blueprint->robots[3].material = GEODE;
        current_blueprint->robots[3].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[3].prices[1] = 0;

        ptr += 9;

        current_blueprint->robots[3].prices[2] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[0].prices[3] = 0;
    }

    fclose(file);

    PointerList *maximum_geodes = initialize_pointerlist_of_capacity(blueprints->size);

    for (int i = 0; i < blueprints->size; i++) {
        Blueprint *current_blueprint = get_pointer(blueprints, i);

        printf("%d\n", current_blueprint->id);

        PointerList *stack = initialize_pointerlist();

        StackElement *start_element = malloc(sizeof(StackElement));
        StackElement start_element_values = {
                MINUTES,
                {0, 0, 0, 0},
                {1, 0, 0, 0}};
        *start_element = start_element_values;

        add_pointer(stack, start_element);

        int maximum_current_geodes = 0;

        while (stack->size > 0) {
            StackElement *current_element = remove_at(stack, stack->size - 1);

//            printf("%d\n", current_element->minutes_left);
            printf("Materials: %d, %d, %d, %d\n", current_element->material_count[0], current_element->material_count[1], current_element->material_count[2], current_element->material_count[3]);
            printf("Robots: %d, %d, %d, %d\n", current_element->robot_count[0], current_element->robot_count[1], current_element->robot_count[2], current_element->robot_count[3]);

            if (current_element->minutes_left == 0) {
                int current_geodes = current_element->material_count[GEODE];

                if (current_geodes > maximum_current_geodes) {
                    maximum_current_geodes = current_geodes;
                }

                free(current_element);
                continue;
            }

            int can_afford_any = 0;

            for (int j = 0; j < ROBOT_AMOUNT; j++) {
                Robot *current_robot = &(current_blueprint->robots[j]);

                int can_afford = 1;
                int minimum_cycles_needed = 1;
                for (int k = 0; k < ROBOT_AMOUNT; k++) {
                    int current_robot_part_count = current_element->robot_count[k];
                    int current_material_count = current_element->material_count[k];
                    int current_robot_part_price = current_robot->prices[k];

                    if (current_robot_part_price > 0 && current_robot_part_count == 0) {
                        can_afford = 0;
                        continue;
                    } else if (current_robot_part_count > 0) {
                        int cycles_needed =
                                (current_robot_part_count - current_material_count + current_robot_part_price - 1) /
                                        current_robot_part_count;
                        if (cycles_needed > minimum_cycles_needed) {
                            minimum_cycles_needed = cycles_needed;
                        }
                    }
                }

                if (can_afford && current_element->minutes_left - minimum_cycles_needed >= 0) {
                    can_afford_any = 1;

                    StackElement *new_element = malloc(sizeof(StackElement));
                    int (*robot_count)[ROBOT_AMOUNT] = &current_element->robot_count;
                    int (*material_count)[ROBOT_AMOUNT] = &current_element->material_count;
                    StackElement new_element_values = {current_element->minutes_left - minimum_cycles_needed,
                                                       {
                                                               (*material_count)[0] +
                                                               (*robot_count)[0] * minimum_cycles_needed -
                                                               current_robot->prices[0],
                                                               (*material_count)[1] +
                                                               (*robot_count)[1] * minimum_cycles_needed -
                                                               current_robot->prices[1],
                                                               (*material_count)[2] +
                                                               (*robot_count)[2] * minimum_cycles_needed -
                                                               current_robot->prices[2],
                                                               (*material_count)[3] +
                                                               (*robot_count)[3] * minimum_cycles_needed -
                                                               current_robot->prices[3],
                                                       }, {
                                                               (*robot_count)[0],
                                                               (*robot_count)[1],
                                                               (*robot_count)[2],
                                                               (*robot_count)[3],
                                                       }};
                    new_element_values.robot_count[j]++;
                    *new_element = new_element_values;
                    add_pointer(stack, new_element);
                }
            }

            if (!can_afford_any) {
                StackElement *new_element = malloc(sizeof(StackElement));
                int (*robot_count)[ROBOT_AMOUNT] = &current_element->robot_count;
                int (*material_count)[ROBOT_AMOUNT] = &current_element->material_count;

                int minimum_cycles_needed = MINUTES - current_element->minutes_left;

                StackElement new_element_values = {0,
                                                   {
                                                           (*material_count)[0] +
                                                           (*robot_count)[0] * minimum_cycles_needed,
                                                           (*material_count)[1] +
                                                           (*robot_count)[1] * minimum_cycles_needed,
                                                           (*material_count)[2] +
                                                           (*robot_count)[2] * minimum_cycles_needed,
                                                           (*material_count)[3] +
                                                           (*robot_count)[3] * minimum_cycles_needed,
                                                   }, {
                                                           (*robot_count)[0],
                                                           (*robot_count)[1],
                                                           (*robot_count)[2],
                                                           (*robot_count)[3],
                                                   }};
                *new_element = new_element_values;
                add_pointer(stack, new_element);
            }

            free(current_element);
        }

        add_int(maximum_geodes, maximum_current_geodes);
    }

    int maximum_geode = 0;

    for (int i = 0; i < maximum_geodes->size; i++) {
        if (maximum_geode < get_int(maximum_geodes, i)) {
            maximum_geode = get_int(maximum_geodes, i);
        }
    }

    printf("%d\n", maximum_geode);

    delete_pointerlist(maximum_geodes);

    delete_pointerlist(blueprints);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

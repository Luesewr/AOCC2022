#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define MINUTES 32

#define ROBOT_AMOUNT 4

typedef enum Material {
    ORE,
    CLAY,
    OBSIDIAN,
    GEODE
} Material;

typedef struct Robot {
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

    file = fopen("../days/day19/input_day19.txt", "r");

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

        current_blueprint->robots[0].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[0].prices[1] = 0;
        current_blueprint->robots[0].prices[2] = 0;
        current_blueprint->robots[0].prices[3] = 0;


        ptr += 28;

        current_blueprint->robots[1].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[1].prices[1] = 0;
        current_blueprint->robots[1].prices[2] = 0;
        current_blueprint->robots[1].prices[3] = 0;


        ptr += 32;

        current_blueprint->robots[2].prices[0] = (int) strtol(ptr, &ptr, 10);

        ptr += 9;

        current_blueprint->robots[2].prices[1] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[2].prices[2] = 0;
        current_blueprint->robots[2].prices[3] = 0;


        ptr += 30;

        current_blueprint->robots[3].prices[0] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[3].prices[1] = 0;

        ptr += 9;

        current_blueprint->robots[3].prices[2] = (int) strtol(ptr, &ptr, 10);
        current_blueprint->robots[3].prices[3] = 0;
    }

    fclose(file);

    PointerList *maximum_geodes = initialize_pointerlist_of_capacity(blueprints->size);

    int blueprints_left = blueprints->size < 3 ? blueprints->size : 3;

    for (int i = 0; i < blueprints_left; i++) {
        Blueprint *current_blueprint = get_pointer(blueprints, i);

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

            if (current_element->minutes_left == 0) {
                int current_geodes = current_element->material_count[GEODE];

                if (current_geodes > maximum_current_geodes) {
                    maximum_current_geodes = current_geodes;
                }

                free(current_element);
                continue;
            }

            int can_afford_any = 0;

            //Can I buy a robot
            for (int j = 0; j < ROBOT_AMOUNT; j++) {
                Robot *current_robot = &(current_blueprint->robots[j]);

                //Can I buy *this* robot
                int can_afford = 1;
                int minimum_cycles_needed = 1;
                for (int k = 0; k < ROBOT_AMOUNT; k++) {
                    int current_robots_owned_count = current_element->robot_count[k];
                    int current_material_count = current_element->material_count[k];
                    int current_robot_part_price = current_robot->prices[k];

                    if (current_robots_owned_count == 0 && current_robot_part_price > 0) {
                        can_afford = 0;
                        break;
                    } else if (current_robot_part_price > 0 && current_robots_owned_count > 0) {
                        int cycles_needed =
                                (current_robot_part_price - current_material_count + current_robots_owned_count - 1) /
                                current_robots_owned_count;
                        if (cycles_needed + 1 > minimum_cycles_needed) {
                            minimum_cycles_needed = cycles_needed + 1;
                        }
                    }
                }

                int new_minutes_left = current_element->minutes_left - minimum_cycles_needed;
                int geode_robots_owned = current_element->robot_count[GEODE];
                int best_possible_geodes_for_current = ((new_minutes_left + geode_robots_owned) * (new_minutes_left + geode_robots_owned + 1)) / 2;

                //If I can buy this robot within the time left I create a new timeline where I bought it.
                if (can_afford && new_minutes_left > 0 && best_possible_geodes_for_current >= (maximum_current_geodes - current_element->material_count[GEODE])) {
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
                int (*robot_count)[ROBOT_AMOUNT] = &current_element->robot_count;

                int minimum_cycles_needed = current_element->minutes_left;


                current_element->material_count[0] += (*robot_count)[0] * minimum_cycles_needed;
                current_element->material_count[1] += (*robot_count)[1] * minimum_cycles_needed;
                current_element->material_count[2] += (*robot_count)[2] * minimum_cycles_needed;
                current_element->material_count[3] += (*robot_count)[3] * minimum_cycles_needed;
                current_element->minutes_left = 0;
                add_pointer(stack, current_element);
            } else {
                free(current_element);
            }
        }

        add_int(maximum_geodes, maximum_current_geodes);
        delete_pointerlist(stack);
    }

    int total_quality_level = 1;

    for (int i = 0; i < blueprints_left; i++) {
        total_quality_level *= get_int(maximum_geodes, i);
    }

    printf("%d\n", total_quality_level);

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

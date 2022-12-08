#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include <time.h>
#include "day7_functions.h"

typedef struct stack_element {
    fs_element *element;
    int indentation;
    int is_end;
} stack_element;

stack_element *create_stack_element(fs_element *element, int indentation, int is_end) {
    stack_element *new_stack_element = malloc(sizeof(stack_element));
    new_stack_element->element = element;
    new_stack_element->indentation = indentation;
    new_stack_element->is_end = is_end;
    return new_stack_element;
}

/*
 * Day 7, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Initialize variable:
     *      -directories, to store all directories in the entire structure.
     *      -files, to store all files in the entire structure.
     *      -root, the top directory.
     */

    PointerList *directories = initialize_pointerlist();
    PointerList *files = initialize_pointerlist();

    fs_element *root = create_fs_element("/\n", D, 0, NULL);

    add_pointer(directories, root);

    /*
     * Parse the input file into the structures.
     */
    parse_input(root, directories, files);

    PointerList *stack = initialize_pointerlist();
    add_pointer(stack, create_stack_element(root, 0, 1));


    while (stack->size > 0) {
        stack_element *cur_dir = remove_at(stack, stack->size - 1);
        if (cur_dir->element->type == D) {
            char padding[3 * cur_dir->indentation + 1];
            for (int i = 0; i < 3 * cur_dir->indentation; i++) {
                padding[i] = ' ';
            }
            padding[3 * cur_dir->indentation] = '\0';
            printf("%s%s", padding, cur_dir->element->name);
            PointerList *children = cur_dir->element->children;
            for (int i = 0; i < children->size; i++) {
                fs_element *current = get_pointer(children, i);
                add_pointer(stack, create_stack_element(current, cur_dir->indentation + 1, i == children->size - 1));
            }
        } else {
            char padding[3 * cur_dir->indentation + 1];
            for (int i = 0; i < 3 * cur_dir->indentation; i++) {
                padding[i] = ' ';
            }
            padding[3 * cur_dir->indentation] = '\0';
            printf("%s-%s", padding, cur_dir->element->name);
        }
        free(cur_dir);
    }

    /*
     * Print the result.
     */

    /*
     * Clean up all allocated memory as well as close the file reader.
     */
    cleanup(directories, files);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);

}

#include "day7_functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Function to create a filesystem element.
 * @param name      Name of the file or directory.
 * @param type      The type of the element, file or directory.
 * @param size      Size of the element.
 * @param parent    Pointer to the directory to which this element belongs.
 * @return          Pointer to the created element.
 */
fs_element *create_fs_element(char *name, fs_type type, int size, fs_element *parent) {

    /*
     * Create the new element and assign all of its elements.
     */
    fs_element *new_fs_element = malloc(sizeof(fs_element));
    new_fs_element->type = type;
    new_fs_element->size = size;
    new_fs_element->parent = parent;
    new_fs_element->children = type == D ? initialize_pointerlist() : NULL;

    /*
     * Copy the string into the name. (strcpy did not want to work)
     */
    for (int i = 0; i < MAX_INPUT_LENGTH; i++) {
        new_fs_element->name[i] = name[i];
    }

    return new_fs_element;
}

/**
 * Finds a directory within the directory that you give with the name that you give.
 * @param dir   The directory to search in.
 * @param name  The name the directory has to have.
 * @return      A pointer to the directory in the directory with the give name.
 */
fs_element *get_dir_by_name(fs_element *dir, char *name) {

    PointerList *children = dir->children;

    /*
     * Loop through all the filesystem elements in the given directory.
     */
    for (int i = 0; i < children->size; i++) {

        fs_element *child = (struct fs_element *) get_pointer(children, i);

        /*
         * Return the current filesystem element being searched if the name matches.
         */
        if (strcmp(child->name, name) == 0) {
            return child;
        }
    }

    /*
     * Return NULL if there wasn't a directory with the right name.
     */
    return NULL;
}

/**
 * Calculates the size of all the files under the given filesystem element.
 * @param element   The element to calculate the size of.
 * @return          The size of the element.
 */
int calculate_size(fs_element *element) {

    /*
     * If the element is a directory, recursively calculate the size of all the elements in it.
     */
    if (element->type == D) {

        int totalSize = 0;
        PointerList *children = element->children;

        for (int i = 0; i < children->size; i++) {
            fs_element *current = get_pointer(children, i);
            totalSize += calculate_size(current);
        }

        element->size = totalSize;
    }

    return element->size;
}

void parse_input(fs_element *root, PointerList *directories, PointerList *files) {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day7/input_day7.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        exit(1);
    }

    char line[25];
    fs_element *cur_dir = root;

    /*
     * Read all the line in the input file.
     */
    while (fgets(line, MAX_INPUT_LENGTH, file) != NULL) {

        /*
         * Make a pointer to the line that we can manipulate without manipulating the actual line variable.
         */
        char *ptr = line;

        /*
         * Check if the directory is being changed.
         */
        if (line[0] == '$' && line[2] == 'c') {
            ptr += 5;
            int go_up = ptr[0] == '.';
            int go_root = ptr[0] == '/';
            cur_dir = !go_up && !go_root ? get_dir_by_name(cur_dir, ptr) : (go_up ? cur_dir->parent : root);
            continue;
        }

        /*
         * Create a new file or directory if there wasn't a command being used in the line. (Meaning it's part of the ls command)
         */
        PointerList *list;
        int size;
        fs_type type;

        /*
         * Change parameters according to if the element is a file or directory.
         */
        if (line[0] == 'd') {
            size = 0;
            ptr += 4;
            list = directories;
            type = D;
        } else {
            size = (int) strtol(ptr, &ptr, 10);
            ptr += 1;
            list = files;
            type = F;
        }

        fs_element *new_dir = create_fs_element(ptr, type, size, cur_dir);
        add_pointer(list, new_dir);
        add_pointer(cur_dir->children, new_dir);
    }

    fclose(file);
}

void cleanup(PointerList *directories, PointerList * files) {
    for (int i = 0; i < directories->size; i++) {
        fs_element *current = (fs_element *) get_pointer(directories, i);
        delete_pointerlist_not_pointers(current->children);
        free(current);
    }

    for (int i = 0; i < files->size; i++) {
        fs_element *current = (fs_element *) get_pointer(files, i);
        free(current);
    }

    delete_pointerlist_not_pointers(directories);
    delete_pointerlist_not_pointers(files);
}
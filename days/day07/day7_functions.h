#include "../../library/pointerList.h"

#ifndef AOCC_DAY7_FUNCTIONS_H
#define AOCC_DAY7_FUNCTIONS_H

#define MAX_INPUT_LENGTH 25

/**
 * Enumerator for filesystem elements.
 *  D -> Directory
 *  F -> File
 */
typedef enum fs_type {
    D, F
} fs_type;

/**
 * Data type for filesystem elements.
 */
typedef struct fs_element {
    fs_type type;
    int size;
    char name[MAX_INPUT_LENGTH];
    struct fs_element *parent;
    PointerList *children;
} fs_element;

fs_element *create_fs_element(char *name, fs_type type, int size, fs_element *parent);

fs_element *get_dir_by_name(fs_element *dir, char *name);

int calculate_size(fs_element *element);

void parse_input(fs_element *root, PointerList *directories, PointerList *files);

void cleanup(PointerList *directories, PointerList * files);

#endif //AOCC_DAY7_FUNCTIONS_H

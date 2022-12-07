#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../library/pointerList.h"
#include <time.h>

/**
 * Directory type to store subdirectories and files in.
 */
typedef struct directory {
    PointerList *children;
} directory;

typedef struct child {
    int type;
    int size;
    char name[25];
    struct child *parent;
    directory *dir;
} child;

child *create_directory(char *name, child *parent) {
    child *new_dir = malloc(sizeof(child));
    new_dir->type = 0;
    new_dir->size = 0;
    new_dir->parent = parent;
    new_dir->dir = malloc(sizeof(directory));
    new_dir->dir->children = initialize_pointerlist();
    for (int i = 0; i < 25; i++) {
        new_dir->name[i] = name[i];
    }
    return new_dir;
}

void add_file(child *dir, child *file) {
    add_pointer(dir->dir->children, file);
}

void add_directory(child *parent, child *child) {
    add_pointer(parent->dir->children, child);
}

child *create_file(char *name, int size, child *parent) {
    child *new_file = malloc(sizeof(child));
    new_file->type = 1;
    new_file->size = size;
    new_file->parent = parent;
    new_file->dir = NULL;
    for (int i = 0; i < 25; i++) {
        new_file->name[i] = name[i];
    }
    return new_file;
}

child *get_dir_by_name(child *dir, char *name) {
    PointerList *children = dir->dir->children;
    int size = children->size;
    for (int i = 0; i < size; i++) {
        child *child = (struct child *) get_pointer(children, i);
        if (strcmp(child->name, name) == 0) {
            return child;
        }
    }
    return NULL;
}

void delete_file(child *file) {
    free(file);
}

void delete_directory(child *dir) {
    free(dir->dir);
    free(dir);
}

int calculate_size(child *child) {
    if (child->type == 0) {
        int totalSize = 0;
        PointerList *children = child->dir->children;
        for (int i = 0; i < children->size; i++) {
            totalSize += calculate_size((struct child *) get_pointer(children, i));
        }
        child->size = totalSize;
    }
    return child->size;
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
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../inputs/input_day7.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    char line[25];

    PointerList *files = initialize_pointerlist();
    PointerList *directories = initialize_pointerlist();

    child *root = create_directory("/\n", NULL);
    add_pointer(directories, root);
    child *cur_dir = root;

    while (fgets(line, 25, file) != NULL) {
        if (line[0] == '$') {
            if (line[2] == 'c') {
                if (line[5] == '.') {
                    cur_dir = cur_dir->parent;
                } else if (line[5] == '/') {
                    cur_dir = root;
                } else {
                    cur_dir = get_dir_by_name(cur_dir, (char *) (((int) line) + 5));
                }
//                printf("Moved to directory: %s", cur_dir->name);
            }
        } else {
            if (line[0] == 'd') {
                child *new_dir = create_directory((char *) (((int) line) + 4), cur_dir);
                add_pointer(directories, new_dir);
                add_directory(cur_dir, new_dir);
//                printf("Added directory: %s", new_dir->name);
            } else {
                char *ptr = line;
                int size = (int) strtol(ptr, &ptr, 10);
                ptr++;
                child *new_file = create_file(ptr, size, cur_dir);
                add_file(cur_dir, new_file);
                add_pointer(files, new_file);
//                printf("Added file: size: %d, name: %s", new_file->size, new_file->name);
            }
        }
    }

    calculate_size(root);

    int result = 0;

    for (int i = 0; i < directories->size; i++) {
        child *current = get_pointer(directories, i);
        if (current->size <= 100000) {
            result += current->size;
        }
    }

    printf("%d\n", result);

    for (int i = 0; i < directories->size; i++) {
        delete_directory((child *) get_pointer(directories, i));
    }

    for (int i = 0; i < files->size; i++) {
        delete_file((child *) get_pointer(files, i));
    }

    delete_pointerlist_not_pointers(directories);
    delete_pointerlist_not_pointers(files);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);

}

#include "pointerList.h"
#include <stdlib.h>
#include <stdio.h>

void add_int(PointerList *pointerList, int value) {
    int *pointer = malloc(sizeof(int) + 1);
    *pointer = value;
    add_pointer(pointerList, pointer);
}

void add_int_at(PointerList *pointerList, int index, int value) {
    int *pointer = malloc(sizeof(int) + 1);
    *pointer = value;
    set_pointer(pointerList, index, pointer);
}

int get_int(PointerList *pointerList, int index) {
    return *(int *) get_pointer(pointerList, index);
}

int get_last_int(PointerList *pointerList) {
    return *(int *) get_pointer(pointerList, pointerList->size - 1);
}

void set_int(PointerList *pointerList, int index, int value) {
    *(int *) get_pointer(pointerList, index) = value;
}

void copy_into_int(void ** array1, int ** array2, int size1, int size2) {
    int smallest = size1 < size2 ? size1 : size2;
    for (int i = 0; i < smallest; i++) {
        array2[i] = (int *) array1[i];
    }
}

void print_ints(PointerList *pointerList) {
    for (int i = 0; i < pointerList->size; i++) {
        printf("%d ", get_int(pointerList, i));
    }
    printf("\n");
}

void print_ints_in_capacity(PointerList *pointerList) {
    for (int i = 0; i < pointerList->capacity; i++) {
        printf("%d ", get_int(pointerList, i));
    }
    printf("\n");
}

int remove_int_at(PointerList *pointerList, int index) {
    int *pointer = (int *) remove_at(pointerList, index);
    int value = *pointer;
    free(pointer);
    return value;
}

int remove_last_int(PointerList *pointerList) {
    return remove_int_at(pointerList, pointerList->size - 1);
}

PointerList *remove_int_if(PointerList *pointerList, int (*predicateFunction)(int)) {
    PointerList *removedPointerList = initialize_pointerlist_of_capacity(pointerList->size);
    int oldSize = pointerList->size;
    printf("size: %d, capacity: %d\n", pointerList->size, oldSize);
    for (int i = 0; i < pointerList->size; i++) {
        int value = get_int(pointerList, i);
        if (predicateFunction(value)) {
            int *pointer = (int *)remove_at(pointerList, i);
            add_int(removedPointerList, * pointer);
            free(pointer);
            i--;
            continue;
        }
    }
    shrink_max_amount(pointerList);

    return removedPointerList;
}

int index_of_int(PointerList *pointerList, int value) {
    for (int i = 0; i < pointerList->size; i++) {
        if (get_int(pointerList, i) == value) {
            return i;
        }
    }
    return -1;
}

int contains_int(PointerList *pointerList, int value) {
    return index_of_int(pointerList, value) != -1;
}

PointerList *indices_of_int(PointerList *pointerList, int value) {
    PointerList *result = initialize_pointerlist();
    for (int i = 0; i < pointerList->size; i++) {
        if (get_int(pointerList, i) == value) {
            add_int(result, i);
        }
    }
    return result;
}

PointerList *int_values_at(PointerList *pointerList, PointerList *indices) {
    PointerList *result = initialize_pointerlist_of_capacity(indices->size);
    for (int i = 0; i < indices->size; i++) {
        add_int(result, get_int(pointerList, get_int(indices, i)));
    }
    return result;
}

PointerList *filter_ints(PointerList *pointerList, int (*predicateFunction)(int)) {
    PointerList *result = initialize_pointerlist(pointerList->size);
    for (int i = 0; i < pointerList->size; i++) {
        int value = get_int(pointerList, i);
        if (predicateFunction(value)) {
            add_int(result, value);
        }
    }
    return result;
}

void apply_to_ints(PointerList *pointerList, int (*function)(int)) {
    for (int i = 0; i < pointerList->size; i++) {
        set_int(pointerList, i, function(get_int(pointerList, i)));
    }
}

PointerList *map_ints(PointerList *pointerList, int (*function)(int)) {
    PointerList *result = initialize_pointerlist_of_capacity(pointerList->size);
    for (int i = 0; i < pointerList->size; i++) {
        add_int(result, function(get_int(pointerList, i)));
    }
    return result;
}

PointerList *unique_ints(PointerList *pointerList) {
    PointerList *result = initialize_pointerlist(pointerList->size);
    for (int i = 0; i < pointerList->size; i++) {
        int value = get_int(pointerList, i);
        if (!contains_int(result, value)) {
            add_int(result, value);
        }
    }

    shrink_max_amount(result);
    return result;
}

void fill_ints(PointerList *pointerList, int size, int value) {
    if (size > pointerList->capacity) {
        expand_to_capacity(pointerList, size);
    }
    for (int i = 0; i < size; i++) {
        add_int_at(pointerList, i, value);
    }

    if (size > pointerList->size) {
        pointerList->size = size;
    }
}

void fill_ints_to_capacity(PointerList *pointerList, int value) {
    for (int i = 0; i < pointerList->capacity; i++) {
        add_int_at(pointerList, i, value);
    }
    pointerList->size = pointerList->capacity;
}

void fill_ints_empties_to_size(PointerList *pointerList, int size, int value) {
    if (size > pointerList->capacity) {
        expand_to_capacity(pointerList, size);
    }
    for (int i = pointerList->size; i < size; i++) {
        if (get_pointer(pointerList, i)) {
            add_int_at(pointerList, i, value);
        }
    }
    pointerList->size = pointerList->capacity;
}

int *get_int_array(PointerList *pointerList) {
    int **array = malloc(sizeof(int) * pointerList->size + 1);
    copy_into_int(pointerList->pointers, array, pointerList->size, pointerList->size);
    int *result = malloc(sizeof(int) * pointerList->size + 1);
    for (int i = 0; i < pointerList->size; i++) {
        result[i] = *array[i];
    }
    free(array);
    return result;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition_ints(int ** arr, int low, int high, int *lp, int (*comparator)(int, int)) {
    if (comparator(*arr[high], *arr[low]) < 0) {
        swap(arr[low], arr[high]);
    }
    // p is the left pivot, and q is the right pivot.
    int j = low + 1;
    int g = high - 1, k = low + 1;
    int p = *arr[low], q = *arr[high];
    while (k <= g) {

        // if pointers are less than the left pivot
        if (comparator(*arr[k], p) < 0) {
            swap(arr[k], arr[j]);
            j++;
        }

            // if pointers are greater than or equal
            // to the right pivot
        else if (comparator(q, *arr[k]) <= 0) {
            while (comparator(q, *arr[g]) < 0 && k < g) {
                g--;
            }
            swap(arr[k], arr[g]);
            g--;
            if (comparator(*arr[k], p) < 0) {
                swap(arr[k], arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    // bring pivots to their appropriate positions.
    swap(arr[low], arr[j]);
    swap(arr[high], arr[g]);

    // returning the indices of the pivots.
    *lp = j; // because we cannot return two pointers
    // from a function.

    return g;
}

void dual_pivot_quick_sort_ints(int ** arr, int low, int high, int (*comparator)(int, int)) {
    if (low < high) {
        // lp means left pivot, and rp means right pivot.
        int lp, rp;
        rp = partition_ints(arr, low, high, &lp, comparator);
        dual_pivot_quick_sort_ints(arr, low, lp - 1, comparator);
        dual_pivot_quick_sort_ints(arr, lp + 1, rp - 1, comparator);
        dual_pivot_quick_sort_ints(arr, rp + 1, high, comparator);
    }
}

int default_comparator(int i, int j) {
    return i - j;
}

void sort_ints(PointerList *pointerList) {
    dual_pivot_quick_sort_ints((int **)pointerList->pointers, 0, pointerList->size - 1, &default_comparator);
}

void sort_ints_with_comparator(PointerList *pointerList, int (*comparator)(int, int)) {
    dual_pivot_quick_sort_ints((int **)pointerList->pointers, 0, pointerList->size - 1, comparator);
}
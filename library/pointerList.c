#include <stdio.h>
#include <stdlib.h>

#include "pointerList.h"

int STANDARD_LIST_SIZE = 4;

PointerList *initialize_pointerlist() {
    return initialize_pointerlist_of_capacity(STANDARD_LIST_SIZE);
}

PointerList *initialize_pointerlist_of_capacity(int initialCapacity) {
    if (initialCapacity <= 0) {
        return NULL;
    }
    PointerList *pointerlist = malloc(sizeof(PointerList) + 1);
    pointerlist->capacity = initialCapacity;
    pointerlist->size = 0;
    pointerlist->pointers = malloc(sizeof(void *) * pointerlist->capacity + 1);
    for (int i = 0; i < pointerlist->capacity; i++) {
        pointerlist->pointers[i] = NULL;
    }
    return pointerlist;
}

PointerList *initialize_pointerlist_from_array(void **array, int size) {
    PointerList *pointerlist = malloc(sizeof(PointerList) + 1);
    pointerlist->capacity = size < STANDARD_LIST_SIZE ? STANDARD_LIST_SIZE : size;
    pointerlist->size = size;
    pointerlist->pointers = malloc(sizeof(void *) * pointerlist->capacity + 1);
    for (int i = 0; i < size; i++) {
        pointerlist->pointers[i] = array[i];
    }
    for (int i = size; i < STANDARD_LIST_SIZE; i++) {
        pointerlist->pointers[i] = 0;
    }
    return pointerlist;
}

void expand_to_capacity(PointerList *pointerlist, int newCapacity) {
    void * *newElements = malloc(sizeof(void *) * newCapacity + 1);
    copy_into(pointerlist->pointers, newElements, pointerlist->capacity, newCapacity);
    for (int i = pointerlist->size; i < newCapacity; i++) {
        newElements[i] = 0;
    }
    free(pointerlist->pointers);
    pointerlist->pointers = newElements;
    pointerlist->capacity = newCapacity;
}

void shrink_to_capacity(PointerList *pointerlist, int newCapacity) {
    void * *newElements = malloc(sizeof(void *) * newCapacity + 1);
    copy_into(pointerlist->pointers, newElements, pointerlist->capacity, newCapacity);
    free(pointerlist->pointers);
    pointerlist->pointers = newElements;
    pointerlist->capacity = newCapacity;
}

void shrink_max_amount(PointerList *pointerList) {
    int newCapacity = pointerList->capacity;

    while (pointerList->size <= newCapacity / 4) {
        newCapacity /= 2;
    }

    shrink_to_capacity(pointerList, newCapacity);
}

int delete_pointerlist(PointerList *pointerlist) {
    for (int i = 0; i < pointerlist->capacity; i++) {
        free(pointerlist->pointers[i]);
    }
    free(pointerlist->pointers);
    free(pointerlist);
    return 0;
}

int delete_pointerlist_not_pointers(PointerList *pointerlist) {
    free(pointerlist->pointers);
    free(pointerlist);
    return 0;
}

int delete_pointers_not_list(PointerList *list) {
    free(list->pointers);
    return 0;
}

void add_pointer(PointerList *pointerlist, void * pointer) {
    if (pointerlist->capacity == pointerlist->size) {
        expand_to_capacity(pointerlist, pointerlist->capacity * 2);
    }
    pointerlist->pointers[pointerlist->size] = pointer;
    pointerlist->size++;
}

void set_pointer(PointerList *pointerList, int index, void *pointer) {
    if (pointerList->capacity <= index) {
        expand_to_capacity(pointerList, index + 1);
    }
    if (index >= pointerList->size) {
        pointerList->size = index + 1;
    }
    if (pointerList->pointers[index])
        free(pointerList->pointers[index]);
    pointerList->pointers[index] = pointer;
}

void add_all_pointers(PointerList *pointerlist, void **values, int size) {
    expand_to_capacity(pointerlist, pointerlist->size + size);
    for (int i = 0; i < size; i++) {
        add_pointer(pointerlist, values[i]);
    }
}

void concat(PointerList *pointerlist1, PointerList *pointerlist2) {
    if (pointerlist2->size == 0) {
        return;
    }

    int newSize = pointerlist1->size + pointerlist2->size;
    if (newSize > pointerlist1->capacity) {
        expand_to_capacity(pointerlist1, newSize);
    }
    for (int i = 0; i < pointerlist2->size; i++) {
        add_pointer(pointerlist1, pointerlist2->pointers[i]);
    }
}

void * get_pointer(PointerList *pointerlist, int index) {
    if (pointerlist == NULL || index < 0 || index >= pointerlist->size) {
        return NULL;
    }
    return pointerlist->pointers[index];
}

void copy_into(void **array1, void **array2, int size1, int size2) {
    int smallest = size1 < size2 ? size1 : size2;
    for (int i = 0; i < smallest; i++) {
        array2[i] = array1[i];
    }
}

void print_pointers(PointerList *pointerlist) {
    for (int i = 0; i < pointerlist->size; i++) {
        printf("%p ", pointerlist->pointers[i]);
    }
    printf("\n");
}

void print_pointers_in_capacity(PointerList *pointerlist) {
    for (int i = 0; i < pointerlist->capacity; i++) {
        printf("%p ", pointerlist->pointers[i]);
    }
    printf("\n");
}

void * remove_at(PointerList *pointerlist, int index) {
    if (index < 0 || index >= pointerlist->size) {
        return NULL;
    }
    void * result = pointerlist->pointers[index];
    for (int i = index + 1; i < pointerlist->size; i++) {
        pointerlist->pointers[i - 1] = pointerlist->pointers[i];
    }
    pointerlist->size--;
    pointerlist->pointers[pointerlist->size] = 0;
    if (pointerlist->size <= pointerlist->capacity / 4 && pointerlist->size > 4) {
        shrink_to_capacity(pointerlist, pointerlist->capacity / 2);
    }
    return result;
}

PointerList *remove_pointer_if(PointerList *pointerList, int (*predicateFunction)(void *)) {
    PointerList *removedPointerList = initialize_pointerlist();
    int offset = 0;
    for (int i = 0; i < pointerList->size; i++) {
        void *value = get_pointer(pointerList, i);
        if (predicateFunction(value)) {
            add_pointer(removedPointerList, value);
            offset++;
        } else {
            pointerList->pointers[i - offset] = pointerList->pointers[i];
        }
    }
    for (int i = pointerList->size - offset; i < pointerList->size; i++) {
        pointerList->pointers[i] = NULL;
    }
    pointerList->size -= offset;

    shrink_to_capacity(pointerList, pointerList->size);

    return removedPointerList;
}

int index_of(PointerList *pointerlist, void * value) {
    for (int i = 0; i < pointerlist->size; i++) {
        if (pointerlist->pointers[i] == value) {
            return i;
        }
    }
    return -1;
}

int contains(PointerList *pointerlist, void * value) {
    return index_of(pointerlist, value) != -1;
}

int contains_with(PointerList *pointerlist, int (*predicate)(void *)) {
    for (int i = 0; i < pointerlist->size; i++) {
        if (predicate(get_pointer(pointerlist, i))) {
            return 1;
        }
    }

    return 0;
}

void swap_pointers(void ** arr, int i, int j) {
    void *temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition_pointers(void ** arr, int low, int high, int *lp, int (*comparator)(void *, void *)) {
    if (comparator(arr[high], arr[low]) < 0) {
        swap_pointers(arr, low, high);
    }
    // p is the left pivot, and q is the right pivot.
    int j = low + 1;
    int g = high - 1, k = low + 1;
    void *p = arr[low], *q = arr[high];
    while (k <= g) {

        // if pointers are less than the left pivot
        if (comparator(arr[k], p) < 0) {
            swap_pointers(arr, k, j);
            j++;
        }

            // if pointers are greater than or equal
            // to the right pivot
        else if (comparator(q, arr[k]) <= 0) {
            while (comparator(q, arr[g]) < 0 && k < g) {
                g--;
            }
            swap_pointers(arr, k, g);
            g--;
            if (comparator(arr[k], p) < 0) {
                swap_pointers(arr, k, j);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    // bring pivots to their appropriate positions.
    swap_pointers(arr, low, j);
    swap_pointers(arr, high, g);

    // returning the indices of the pivots.
    *lp = j; // because we cannot return two pointers
    // from a function.

    return g;
}

void dual_pivot_quick_sort_pointers(void ** arr, int low, int high, int (*comparator)(void *, void *)) {
    if (low < high) {
        // lp means left pivot, and rp means right pivot.
        int lp, rp;
        rp = partition_pointers(arr, low, high, &lp, comparator);
        dual_pivot_quick_sort_pointers(arr, low, lp - 1, comparator);
        dual_pivot_quick_sort_pointers(arr, lp + 1, rp - 1, comparator);
        dual_pivot_quick_sort_pointers(arr, rp + 1, high, comparator);
    }
}

void sort_pointers_with_comparator(PointerList *pointerList, int (*comparator)(void *, void *)) {
    dual_pivot_quick_sort_pointers((void **)pointerList->pointers, 0, pointerList->size - 1, comparator);
}
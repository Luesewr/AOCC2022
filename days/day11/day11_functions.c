#include "../../library/pointerList.h"
#include "day11_functions.h"
#include <stdlib.h>

void add_long(PointerList *pointerList, long long value) {
    long long *pointer = malloc(sizeof(long long) + 1);
    *pointer = value;
    add_pointer(pointerList, pointer);
}

void set_long(PointerList *pointerList, int index, long long value) {
    *(long long *) get_pointer(pointerList, index) = value;
}

long long get_long(PointerList *pointerList, int index) {
    return *(long long *) get_pointer(pointerList, index);
}

void swap_long(long *a, long *b) {
    long temp = *a;
    *a = *b;
    *b = temp;
}

int partition_long(long ** arr, int low, int high, long *lp, long (*comparator)(long, long)) {
    if (comparator(*arr[high], *arr[low]) < 0) {
        swap_long(arr[low], arr[high]);
    }
    // p is the left pivot, and q is the right pivot.
    int j = low + 1;
    int g = high - 1, k = low + 1;
    long p = *arr[low], q = *arr[high];
    while (k <= g) {

        // if pointers are less than the left pivot
        if (comparator(*arr[k], p) < 0) {
            swap_long(arr[k], arr[j]);
            j++;
        }

            // if pointers are greater than or equal
            // to the right pivot
        else if (comparator(q, *arr[k]) <= 0) {
            while (comparator(q, *arr[g]) < 0 && k < g) {
                g--;
            }
            swap_long(arr[k], arr[g]);
            g--;
            if (comparator(*arr[k], p) < 0) {
                swap_long(arr[k], arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    // bring pivots to their appropriate positions.
    swap_long(arr[low], arr[j]);
    swap_long(arr[high], arr[g]);

    // returning the indices of the pivots.
    *lp = j; // because we cannot return two pointers
    // from a function.

    return g;
}

void dual_pivot_quick_sort_long(long ** arr, int low, int high, long (*comparator)(long, long)) {
    if (low < high) {
        // lp means left pivot, and rp means right pivot.
        long lp, rp;
        rp = partition_long(arr, low, high, &lp, comparator);
        dual_pivot_quick_sort_long(arr, low, lp - 1, comparator);
        dual_pivot_quick_sort_long(arr, lp + 1, rp - 1, comparator);
        dual_pivot_quick_sort_long(arr, rp + 1, high, comparator);
    }
}

void sort_long_with_comparator(PointerList *pointerList, long (*comparator)(long, long)) {
    dual_pivot_quick_sort_long((long **)pointerList->pointers, 0, pointerList->size - 1, comparator);
}

long reverseLongComparator(long i, long j) {
    return j - i;
}

int reverseIntComparator(int i, int j) {
    return j - i;
}

/**
 * Loops in a string to find the size of the string before it reaches a newline character.
 * @param array The character array (string) with a newline character.
 * @return the size of the string before it reached a newline character.
 */
int findEnd(const char * array) {
    int i = 0;
    do {
        i++;
    } while (array[i] != '\n');
    return i;
}
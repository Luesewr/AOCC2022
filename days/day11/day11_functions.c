#include "../../library/pointerList.h"
#include "day11_functions.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Parses the input file into a list of monkeys.
 * @param monkeys   A list of monkeys.
 * @return  The smallest common denominator of the test in each monkey.
 */
long long parse_input(PointerList *monkeys) {
    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day11/input_day11.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */
    if (file == NULL) {
        exit(1);
    }

    /*
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  total_divider, which will keep track of the smallest common denominator.
     */
    char line[55];
    long long total_divider = 1;

    /*
     * Loop over the input file.
     */
    while (fgets(line, 55, file) != NULL) {
        /*
         * Create a monkey.
         */
        Monkey *monkey = malloc(sizeof(Monkey));

        /*
         * Parse starting items.
         */
        fgets(line, 55, file);
        char *ptr = line + 18;
        int itemAmount = (findEnd(ptr) + 2) / 4;

        monkey->items = initialize_pointerlist();

        for (int i = 0; i < itemAmount; i++) {
            add_long(monkey->items, strtol(ptr, &ptr, 10));
            ptr += 2;
        }

        /*
         * Parse function.
         */
        fgets(line, 55, file);
        Function *function = malloc(sizeof(Function));
        ptr = line + 19;

        if (ptr[0] == 'o') {
            function->type1 = VARIABLE;
            function->data1 = 0;
            ptr += 4;
        } else {
            function->type1 = NUMBER;
            function->data1 = strtol(ptr, &ptr, 10);
            ptr += 1;
        }

        function->operator = ptr[0] == '*';
        ptr += 2;

        if (ptr[0] == 'o') {
            function->type2 = VARIABLE;
            function->data2 = 0;
        } else {
            function->type2 = NUMBER;
            function->data2 = strtol(ptr, &ptr, 10);
        }

        monkey->function = function;

        /*
         * Parse test line.
         */
        fgets(line, 55, file);
        ptr = line + 21;

        monkey->divisible_by = strtol(ptr, &ptr, 10);
        if (total_divider % monkey->divisible_by != 0) {
            total_divider *= monkey->divisible_by;
        }

        /*
         * Parse the true monkey index line.
         */
        fgets(line, 55, file);
        ptr = line + 29;

        monkey->true_index = strtol(ptr, &ptr, 10);

        /*
         * Parse the false monkey index line.
         */
        fgets(line, 55, file);
        ptr = line + 30;

        monkey->false_index = strtol(ptr, &ptr, 10);

        /*
         * Skip the empty line and add the created monkey to the list.
         */
        fgets(line, 55, file);

        add_pointer(monkeys, monkey);
    }

    /*
     * Close the file reader and return the smallest common denominator.
     */
    fclose(file);

    return total_divider;
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

/*
 * Some int list functions translated for the long long-type, because we are dealing with very large numbers.
 */

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

void print_long(PointerList *pointerList) {
    for (int i = 0; i < pointerList->size; i++) {
        printf("%lld ", get_long(pointerList, i));
    }
    printf("\n");
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
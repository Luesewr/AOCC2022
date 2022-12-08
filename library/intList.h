#include "pointerList.h"

#ifndef POINTERLIST_INTLIST_H
#define POINTERLIST_INTLIST_H

void add_int(PointerList *pointerList, int value);

void add_int_at(PointerList *pointerList, int index, int value);

int get_int(PointerList *pointerList, int index);

int get_last_int(PointerList *pointerList);

void set_int(PointerList *pointerList, int index, int value);

void copy_into_int(void ** array1, int ** array2, int size1, int size2);

void print_ints(PointerList *pointerList);

void print_ints_in_capacity(PointerList *pointerList);

int remove_int_at(PointerList *pointerList, int index);

int remove_last_int(PointerList *pointerList);

PointerList *remove_int_if(PointerList *pointerList, int (*predicateFunction)(int));

int index_of_int(PointerList *pointerList, int value);

int contains_int(PointerList *pointerList, int value);

PointerList *indices_of_int(PointerList *pointerList, int value);

PointerList *int_values_at(PointerList *pointerList, PointerList *indices);

PointerList *filter_ints(PointerList *pointerList, int (*predicateFunction)(int));

void apply_to_ints(PointerList *pointerList, int (*function)(int));

PointerList *map_ints(PointerList *pointerList, int (*function)(int));

PointerList *unique_ints(PointerList *pointerList);

void fill_ints(PointerList *pointerList, int size, int value);

void fill_ints_to_capacity(PointerList *pointerList, int value);

void fill_ints_empties_to_size(PointerList *pointerList, int size, int value);

int *get_int_array(PointerList *pointerList);

void sort_ints(PointerList *pointerList);

void sort_ints_with_comparator(PointerList *pointerList, int (*comparator)(int, int));

void swap(int *a, int *b);

void dual_pivot_quick_sort_ints(int ** arr, int low, int high, int (*comparator)(int, int));

int partition_ints(int ** arr, int low, int high, int *lp, int (*comparator)(int, int));

int default_comparator(int i, int j);

#endif //POINTERLIST_INTLIST_H

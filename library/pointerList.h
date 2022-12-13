#ifndef LIST_POINTERLIST_H
#define LIST_POINTERLIST_H

typedef struct {
    void **pointers;
    int size;
    int capacity;
} PointerList;

PointerList *initialize_pointerlist();

PointerList *initialize_pointerlist_of_capacity(int initialCapacity);

PointerList *initialize_pointerlist_from_array(void **array, int size);

void expand_to_capacity(PointerList *pointerlist, int newCapacity);

void shrink_to_capacity(PointerList *pointerlist, int newCapacity);

void shrink_max_amount(PointerList *pointerList);

int delete_pointerlist(PointerList *pointerlist);

int delete_pointerlist_not_pointers(PointerList *pointerlist);

void add_pointer(PointerList *pointerlist, void * pointer);

void set_pointer(PointerList *pointerList, int index, void *pointer);

void add_all_pointers(PointerList *pointerlist, void * *values, int size);

void concat(PointerList *pointerlist1, PointerList *pointerlist2);

void * get_pointer(PointerList *pointerlist, int index);

void copy_into(void **array1, void **array2, int size1, int size2);

void print_pointers(PointerList *pointerlist);

void print_pointers_in_capacity(PointerList *pointerlist);

void * remove_at(PointerList *pointerlist, int index);

int index_of(PointerList *pointerlist, void * value);

int contains(PointerList *pointerlist, void * value);

void swap_pointers(void ** arr, int i, int j);

int partition_pointers(void ** arr, int low, int high, int *lp, int (*comparator)(void *, void *));

void dual_pivot_quick_sort_pointers(void ** arr, int low, int high, int (*comparator)(void *, void *));

void sort_pointers_with_comparator(PointerList *pointerList, int (*comparator)(void *, void *));

#endif //LIST_POINTERLIST_H


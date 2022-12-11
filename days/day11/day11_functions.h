//
// Created by Laure on 2022-12-11.
//

#ifndef POINTERLIST_DAY11_FUNCTIONS_H
#define POINTERLIST_DAY11_FUNCTIONS_H

#include "../../library/pointerList.h"

typedef enum funcType{VARIABLE, NUMBER} funcType;

typedef struct LongFunction {
    funcType type1;
    long long data1;
    int operator;
    funcType type2;
    long long data2;
} LongFunction;

typedef struct LongMonkey {
    PointerList *items;
    LongFunction *function;
    int divisible_by;
    int true_index;
    int false_index;
} LongMonkey;

typedef struct IntFunction {
    funcType type1;
    int data1;
    int operator;
    funcType type2;
    int data2;
} IntFunction;

typedef struct IntMonkey {
    PointerList *items;
    IntFunction *function;
    int divisible_by;
    int true_index;
    int false_index;
} IntMonkey;

void add_long(PointerList *pointerList, long long value);

void set_long(PointerList *pointerList, int index, long long value);

long long get_long(PointerList *pointerList, int index);

void swap_long(long *a, long *b);

int partition_long(long ** arr, int low, int high, long *lp, long (*comparator)(long, long));

void dual_pivot_quick_sort_long(long ** arr, int low, int high, long (*comparator)(long, long));

void sort_long_with_comparator(PointerList *pointerList, long (*comparator)(long, long));

long reverseLongComparator(long i, long j);

int reverseIntComparator(int i, int j);

int findEnd(const char * array);

#endif //POINTERLIST_DAY11_FUNCTIONS_H

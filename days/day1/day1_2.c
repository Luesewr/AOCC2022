//
// Created by Laure on 12/1/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

int reverseComparator(int i, int j) {
    return j - i;
}

int main() {
    FILE *file;

    file = fopen("../inputs/input_day1_1.txt", "r");

    if (file == NULL) {
        return 1;
    }

    PointerList * list = initialize_pointerlist();

    int maxTotal = 0, curTotal = 0, input;
    char line[20];
    while (fgets(line, 20, file) != NULL) {
        if (line[0] != '\n') {
            input = atoi(line);
            curTotal += input;
        } else {
            add_int(list, curTotal);
            curTotal = 0;
        }
    }
    sort_ints_with_comparator(list, &reverseComparator);

    int topThreeTotal = 0;
    for (int i = 0; i < 3; i++) {
        topThreeTotal += get_int(list, i);
    }

    fclose(file);
    delete_pointerlist(list);
    printf("%d\n", topThreeTotal);
}
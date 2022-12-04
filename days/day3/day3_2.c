#include <stdio.h>
#include "../../library/pointerList.h"
#include "../../library/intList.h"

#define MAX_ENTRY_SIZE 50

int findEnd(const char * array) {
    int i = MAX_ENTRY_SIZE;
    do  {
        i--;
    } while (array[i] != '\n');
    return i;
}

int mapToValue(int x) {
    return x - ((x > 92) * 96) - ((x < 92) * 38);
}

int orderComparator(int i, int j) {
    return mapToValue(i) - mapToValue(j);
}

int index_of_int_with_cutoff(PointerList *pointerList, int value, int * offset) {
    for (int i = *offset; i < pointerList->size; i++) {
        int current = get_int(pointerList, i);
        int result = orderComparator(current, value);
        if (!result) {
            *offset = i;
            return i;
        } else if (result > 0) {
            if (i > *offset + 1) {
                *offset = i - 1;
            }
            return -1;
        }
    }
    return -1;
}

int contains_int_with_cutoff(PointerList *pointerList, int value, int * offset) {
    return index_of_int_with_cutoff(pointerList, value, offset) != -1;
}

int main() {
    FILE *file;

    file = fopen("../inputs/input_day3.txt", "r");

    if (file == NULL) {
        return 1;
    }

    int totalScore = 0;
    char line[MAX_ENTRY_SIZE];
    PointerList * listList = initialize_pointerlist_of_capacity(3);
    int j = 0;
    while (fgets(line, MAX_ENTRY_SIZE, file)) {
        int length = findEnd(line);
        PointerList * list = initialize_pointerlist_of_capacity(length);
        for (int i = 0; i < length; i++) {
            add_int(list, (int) line[i]);
        }
        sort_ints_with_comparator(list, &orderComparator);
        add_pointer(listList, list);
        for (int i = 0; i <= length; i++) {
            line[i] = 0;
        }
        j++;
        if (j % 3 == 0) {
            PointerList * firstList = ((PointerList *) get_pointer(listList, 0));
            int offset2 = 0, offset3 = 0;
            for (int i = 0, len = firstList->size; i < len; i++) {
                int current = get_int(firstList, i);
                int secondContains = contains_int_with_cutoff((PointerList *) get_pointer(listList, 1), current, &offset2);
                int thirdContains = contains_int_with_cutoff((PointerList *) get_pointer(listList, 2), current, &offset3);
                if (secondContains && thirdContains) {
                    totalScore += mapToValue(current);
                    break;
                }
            }
            for (int i = 0; i < 3; i++) {
                delete_pointerlist(get_pointer(listList, i));
                listList->pointers[i] = NULL;
            }
            listList->size = 0;
        }
    }

    fclose(file);

    printf("%d\n", totalScore);
}
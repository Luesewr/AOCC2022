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

int main() {
    FILE *file;

    file = fopen("../inputs/input_day3.txt", "r");

    if (file == NULL) {
        return 1;
    }

    int totalScore = 0;
    char line[MAX_ENTRY_SIZE];
    while (fgets(line, MAX_ENTRY_SIZE, file)) {
        int length = findEnd(line);
        int halfLength = length / 2;
        PointerList * list1 = initialize_pointerlist_of_capacity(halfLength), *list2 = initialize_pointerlist_of_capacity(halfLength);
        for (int i = 0; i < halfLength; i++) {
            add_int(list1, (int) line[i]);
        }
        for (int i = halfLength; i < length; i++) {
            add_int(list2, (int) line[i]);
        }
        sort_ints_with_comparator(list1, &orderComparator);
        sort_ints_with_comparator(list2, &orderComparator);
        int value;
        for (int i = 0;; i++) {
            int current = get_int(list1, i);
            if (contains_int(list2, current)) {
                value = current;
                break;
            }
        }
        totalScore += mapToValue(value);
        delete_pointerlist( list1);
        delete_pointerlist(list2);
        for (int i = 0; i <= length; i++) {
            line[i] = 0;
        }
    }

    fclose(file);

    printf("%d\n", totalScore);
}
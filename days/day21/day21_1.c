#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../library/pointerList.h"

#define NAME_LENGTH 4

typedef enum MonkeyType {
    NUMBER,
    OPERATOR
} MonkeyType;

typedef struct TempOperatorMonkey {
    char variable1[NAME_LENGTH];
    char operator;
    char variable2[NAME_LENGTH];
} TempOperatorMonkey;

typedef struct OperatorMonkey {
    int index1;
    char operator;
    int index2;
} OperatorMonkey;

typedef struct NumberMonkey {
    int number;
} NumberMonkey;

typedef struct Monkey {
    char name[NAME_LENGTH];
    MonkeyType type;
    void *data;
} Monkey;

int name_equals(const char name1[NAME_LENGTH], const char name2[NAME_LENGTH]) {
    int is_equal = 1;

    for (int i = 0; i < NAME_LENGTH; i++) {
        if (name1[i] != name2[i]) {
            is_equal = 0;
            break;
        }
    }

    return is_equal;
}

long long int get_value(PointerList *monkeys, int index) {
    Monkey *current_monkey = get_pointer(monkeys, index);
    if (current_monkey->type == NUMBER) {
        return ((NumberMonkey *) current_monkey->data)->number;
    }

    OperatorMonkey *current_operator_monkey = current_monkey->data;
    long long int value1 = get_value(monkeys, current_operator_monkey->index1);
    long long int value2 = get_value(monkeys, current_operator_monkey->index2);

    switch (current_operator_monkey->operator) {
        case '+':
            return value1 + value2;
        case '-':
            return value1 - value2;
        case '*':
            return value1 * value2;
        case '/':
            return value1 / value2;
    }
    return 0;
}

/*
 * Day 21, Part 1
 */
int main() {

    /*
     * Set up timer.
     */

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /*
     * Setting up the input file.
     */

    FILE *file;

    file = fopen("../days/day21/input_day21.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Create variables:
     *  line, to store the input line.
     */
    char line[20];
    char root_name[NAME_LENGTH] = {'r', 'o', 'o', 't'};
    int root_index = -1;
    PointerList *monkeys = initialize_pointerlist();
    PointerList *operator_monkeys = initialize_pointerlist();

    while (fgets(line, 20, file) != NULL) {
        Monkey *new_monkey = malloc(sizeof(Monkey));

        int is_root = 1;

        for (int i = 0; i < NAME_LENGTH; i++) {
            new_monkey->name[i] = line[i];
            if (line[i] != root_name[i]) {
                is_root = 0;
            }
        }

        if (is_root) {
            root_index = monkeys->size;
        }

        char *ptr = line + 6;

        if (ptr[0] < 'a') {
            new_monkey->type = NUMBER;

            NumberMonkey *new_number_monkey = malloc(sizeof(NumberMonkey));
            new_number_monkey->number = (int) strtol(ptr, NULL, 10);

            new_monkey->data = new_number_monkey;
        } else {
            new_monkey->type = OPERATOR;

            TempOperatorMonkey *new_operator_monkey = malloc(sizeof(TempOperatorMonkey));

            for (int i = 0; i < NAME_LENGTH; i++) {
                new_operator_monkey->variable1[i] = ptr[i];
            }

            new_operator_monkey->operator = ptr[5];

            ptr += 7;

            for (int i = 0; i < NAME_LENGTH; i++) {
                new_operator_monkey->variable2[i] = ptr[i];
            }

            new_monkey->data = new_operator_monkey;
            add_pointer(operator_monkeys, new_monkey);
        }

        add_pointer(monkeys, new_monkey);
    }

    for (int i = 0; i < operator_monkeys->size; i++) {
        Monkey *current_monkey = get_pointer(operator_monkeys, i);
        TempOperatorMonkey *current_operator_monkey = current_monkey->data;

        OperatorMonkey *new_operator_monkey = malloc(sizeof(OperatorMonkey));

        new_operator_monkey->operator = current_operator_monkey->operator;

        for (int j = 0; j < monkeys->size; j++) {
            Monkey *current = get_pointer(monkeys, j);
            if (name_equals(current->name, current_operator_monkey->variable1)) {
                new_operator_monkey->index1 = j;
                break;
            }
        }

        for (int j = 0; j < monkeys->size; j++) {
            Monkey *current = get_pointer(monkeys, j);
            if (name_equals(current->name, current_operator_monkey->variable2)) {
                new_operator_monkey->index2 = j;
                break;
            }
        }

        current_monkey->data = new_operator_monkey;

        free(current_operator_monkey);
    }

    long long int result = get_value(monkeys, root_index);

    printf("%lld\n", result);

    for (int i = 0; i < monkeys->size; i++) {
        Monkey *current = get_pointer(monkeys, i);
        free(current->data);
    }

    delete_pointerlist(monkeys);

    /*
    * Close the timer and print the taken time.
    */
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}

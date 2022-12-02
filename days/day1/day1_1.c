#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;

    file = fopen("../inputs/input_day1.txt", "r");

    if (file == NULL) {
        return 1;
    }

    int maxTotal = 0, curTotal = 0, input;
    char line[20];
    while (fgets(line, 20, file) != NULL) {
        if (line[0] != '\n') {
            input = atoi(line);
            curTotal += input;
        } else {
            if (curTotal > maxTotal) {
                maxTotal = curTotal;
            }
            curTotal = 0;
        }
    }
    fclose(file);
    printf("%d\n", maxTotal);
}
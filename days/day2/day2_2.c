#include <stdio.h>

int main() {
    FILE *file;

    file = fopen("../inputs/input_day2.txt", "r");

    if (file == NULL) {
        return 1;
    }

    int totalScore = 0;

    char line[7];
    while (fgets(line, 6, file)) {
        int input1 = line[0] - (int) 'A', input2 = line[2] - (int) 'X';
        int a = (input1 + input2) % 3;
        totalScore += ((a == 0) * 3 + a) + (input2 * 3);
    }

    printf("%d\n", totalScore);
}

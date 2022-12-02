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
        totalScore += ((input2 - input1 + 3) % 3 * 3 + 3) % 9 + input2 + 1;
    }

    printf("%d\n", totalScore);
}

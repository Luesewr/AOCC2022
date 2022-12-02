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
        int score = 0;
        int input1, input2;
        if (line[0] == 'A') {
            input1 = 0;
        } else if (line[0] == 'B') {
            input1 = 1;
        } else {
            input1 = 2;
        }
        if (line[2] == 'X') {
            input2 = 0;
        } else if (line[2] == 'Y') {
            input2 = 1;
        } else {
            input2 = 2;
        }
        score += ((input2 - input1 + 3) % 3 * 3 + 3) % 9 + input2 + 1;
        totalScore += score;
    }

    printf("%d\n", totalScore);
}

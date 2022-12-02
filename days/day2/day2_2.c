#include <stdio.h>

int main() {
    FILE *file;

    file = fopen("../inputs/input_day2.txt", "r");

    if (file == NULL) {
        return 1;
    }

    int totalScore = 0;

    char line[6];
    while (fgets(line, 5, file)) {
        int score = 0;
        int rock1 = 0, paper1 = 0, scissors1 = 0, lose = 0, draw = 0, win = 0;
        int input1 = 2, input2 = 2;
        if (line[0] == 'A') {
            input1 = 0;
        } else if (line[0] == 'B') {
            input1 = 1;
        }
        if (line[2] == 'X') {
            input2 = 0;
        } else if (line[2] == 'Y') {
            input2 = 1;
        }


        int a = (input1 + input2) % 3;
        score += (a == 0) * 3 + a + input2 * 3;
        totalScore += score;
    }

    printf("%d\n", totalScore);
}

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
        if (line[0] == 'A') {
            rock1 = 1;
        } else if (line[0] == 'B') {
            paper1 = 1;
        } else if (line[0] == 'C') {
            scissors1 = 1;
        }
        if (line[2] == 'X') {
            lose = 1;
        } else if (line[2] == 'Y') {
            draw = 1;
        } else if (line[2] == 'Z') {
            win = 1;
        }
        if ((win && rock1) || (draw && paper1) || (lose && scissors1)) {
            score += 2;
        } else if ((win && paper1) || (draw && scissors1) || (lose && rock1)) {
            score += 3;
        } else {
            score += 1;
        }
        if (draw) {
            score += 3;
        } else if (win) {
            score += 6;
        }
        totalScore += score;
    }

    printf("%d\n", totalScore);
}

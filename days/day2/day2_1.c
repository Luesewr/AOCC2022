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
        int rock1 = 0, paper1 = 0, scissors1 = 0, rock2 = 0, paper2 = 0, scissors2 = 0;
        if (line[0] == 'A') {
            rock1 = 1;
        } else if (line[0] == 'B') {
            paper1 = 1;
        } else if (line[0] == 'C') {
            scissors1 = 1;
        }
        if (line[2] == 'X') {
            rock2 = 1;
        } else if (line[2] == 'Y') {
            paper2 = 1;
        } else if (line[2] == 'Z') {
            scissors2 = 1;
        }
        if ((rock2 && scissors1) || (paper2 && rock1) || (scissors2 && paper1)) {
            score += 6;
        } else if ((rock2 && rock1) || (paper1 && paper2) || (scissors1 && scissors2)) {
            score += 3;
        }
        if (rock2) {
            score += 1;
        } else if (paper2) {
            score += 2;
        } else if (scissors2) {
            score += 3;
        }
        totalScore += score;
    }

    printf("%d\n", totalScore);
}

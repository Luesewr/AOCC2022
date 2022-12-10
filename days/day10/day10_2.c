#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 6

void draw_to_screen(int cycle, int X, char ** screen) {
    int CRT_pos = cycle % 40;
    char value = CRT_pos >= X - 1 && CRT_pos <= X + 1 ? '#' : '.';
    screen[cycle / 40][CRT_pos] = value;
}

/*
 * Day 10, Part 2
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

    file = fopen("../inputs/input_day10.txt", "r");

    /*
     * Shutdown program if the file can't be found or another error occurred.
     */

    if (file == NULL) {
        return 1;
    }

    /*
     * Set up a line variable which will store the line that was read from the file.
     * Make variables:
     *  containsCount, to keep track of how many ranges one range fully overlapped another.
     *  inputs, to store the integers that were on the line.
     *  ptr, to keep track of which part of the input line were parsing.
     *
     * Then loop over all lines.
     */

    char line[12];
    char ** screen = malloc(sizeof(char *) * HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        screen[i] = malloc(sizeof(char) * (WIDTH + 1));
        screen[i][WIDTH] = '\0';
    }
    int cycle = 0;
    int X = 1;

    while (fgets(line, 12, file) != NULL) {
        draw_to_screen(cycle, X, screen);
        cycle += 1;
        if (line[0] == 'a') {
            char *ptr = line + 5;
            int amount = strtol(ptr, NULL, 10);
            draw_to_screen(cycle, X, screen);
            X += amount;
            cycle += 1;
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        printf("%s\n", screen[i]);
        free(screen[i]);
    }
    free(screen);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
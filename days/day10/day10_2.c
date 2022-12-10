#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 6

/**
 * Set the pixel at the current CRT pixel (the cycle) to the right value according to the X.
 * @param cycle     The current pixel that the CRT monitor is writing to.
 * @param X         The horizontal position.
 * @param screen    The 2 dimensional character array where the screen is stored in.
 */
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
     * Make variables:
     *  line, which will store the line that was read from the file.
     *  cycle, to keep track of the current cycle.
     *  X, to keep track of the X value.
     *  screen, to store all the pixels in the screen.
     */

    char line[12];

    int cycle = 0;
    int X = 1;

    char ** screen = malloc(sizeof(char *) * HEIGHT);

    /*
     * Initialize all the arrays in the screen.
     */
    for (int i = 0; i < HEIGHT; i++) {
        screen[i] = malloc(sizeof(char) * (WIDTH + 1));
        screen[i][WIDTH] = '\0';
    }

    /*
     * Loop over all the lines in the input.
     */
    while (fgets(line, 12, file) != NULL) {

        /*
         * Update the screen with the current state.
         */
        draw_to_screen(cycle, X, screen);
        cycle += 1;

        /*
         * If the input is an addx command, read the amount, update the screen and the X position.
         */
        if (line[0] == 'a') {

            char *ptr = line + 5;
            int amount = strtol(ptr, NULL, 10);

            draw_to_screen(cycle, X, screen);

            X += amount;

            cycle += 1;
        }
    }

    /*
     * Print the screen and free the arrays in the screen variable at the same time.
     */
    for (int i = 0; i < HEIGHT; i++) {
        printf("%s\n", screen[i]);
        free(screen[i]);
    }

    /*
     * Deallocate the screen.
     */
    free(screen);

    /*
     * Close the timer and print the taken time.
     */

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The elapsed time is %f seconds\n", time_spent);
}
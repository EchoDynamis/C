#include <time.h>
#include <stdio.h>
#include <stdlib.h> // For rand() and srand()
#include <unistd.h> // For usleep() (for animation delay)

#define GRID_ROWS 20
#define GRID_COLS 40
#define LIVE_CELL '#'
#define DEAD_CELL ' '
#define DELAY_MICROSECONDS 100000 // 0.1 seconds

// Global grids (or pass them as arguments)
char current_grid[GRID_ROWS][GRID_COLS];
char next_grid[GRID_ROWS][GRID_COLS];

// Function to initialize the grid randomly
void initialize_grid_random() {
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            current_grid[i][j] = (rand() % 5 == 0) ? LIVE_CELL : DEAD_CELL; // ~20% chance of being live
        }
    }
}

// Function to count live neighbors for a given cell (row, col)
int count_live_neighbors(int row, int col) {
    int live_neighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Don't count the cell itself

            int neighbor_row = row + i;
            int neighbor_col = col + j;

            // Handle wrapping boundaries (toroidal array)
            if (neighbor_row < 0) neighbor_row = GRID_ROWS - 1;
            if (neighbor_row >= GRID_ROWS) neighbor_row = 0;
            if (neighbor_col < 0) neighbor_col = GRID_COLS - 1;
            if (neighbor_col >= GRID_COLS) neighbor_col = 0;

            if (current_grid[neighbor_row][neighbor_col] == LIVE_CELL) {
                live_neighbors++;
            }
        }
    }
    return live_neighbors;
}

// Function to update the grid to the next generation
void update_grid() {
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            int neighbors = count_live_neighbors(i, j);

            if (current_grid[i][j] == LIVE_CELL) {
                if (neighbors < 2 || neighbors > 3) {
                    next_grid[i][j] = DEAD_CELL; // Underpopulation or Overpopulation
                } else {
                    next_grid[i][j] = LIVE_CELL; // Survival
                }
            } else { // Current cell is DEAD
                if (neighbors == 3) {
                    next_grid[i][j] = LIVE_CELL; // Reproduction
                } else {
                    next_grid[i][j] = DEAD_CELL; // Stays dead
                }
            }
        }
    }

    // Copy next_grid to current_grid for the next iteration
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            current_grid[i][j] = next_grid[i][j];
        }
    }
}

// Function to print the grid (ASCII visualization)
void print_grid() {
    // Clear the console (system-dependent, for a "refreshing" animation)
    // For Windows: system("cls");
    // For Linux/macOS: system("clear");
    printf("\033[H\033[J"); // ANSI escape codes to clear screen and move cursor to home

    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            printf("%c", current_grid[i][j]);
        }
        printf("\n");
    }
    printf("\nPress Ctrl+C to exit.\n");
}

int main() {
    initialize_grid_random();

    while (1) { // Infinite loop for continuous simulation
        print_grid();
        update_grid();
        usleep(DELAY_MICROSECONDS); // Delay for animation
    }

    return 0;
}

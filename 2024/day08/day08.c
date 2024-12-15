#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 8
// Link: <a href="...">https://adventofcode.com/2024/day/8</a>
////////////////////////////////////////////////////////////////

#define filename "day08test.txt"
//#define filename "day08input.txt"

#define MAX_ROWS 100
#define MAX_COLS 100

// Structure to hold antenna coordinates and frequency
typedef struct {
    int x, y;
    char frequency;
} Antenna;

// Function to calculate Manhattan distance
int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    char map[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    Antenna antennas[MAX_ROWS * MAX_COLS];
    int antenna_count = 0;

    // Read input from file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    while (fgets(map[rows], sizeof(map[0]), file)) {
        map[rows][strcspn(map[rows], "\n")] = '\0'; // Remove newline
        cols = strlen(map[rows]);
        for (int col = 0; col < cols; col++) {
            if (isalnum(map[rows][col])) {
                antennas[antenna_count++] = (Antenna){col, rows, map[rows][col]};
            }
        }
        rows++;
    }
    fclose(file);

    // Use a grid to mark antinodes
    int antinodes[MAX_ROWS][MAX_COLS] = {0};

    // Calculate antinodes for each pair of antennas with the same frequency
    for (int i = 0; i < antenna_count; i++) {
        for (int j = i + 1; j < antenna_count; j++) {
            if (antennas[i].frequency == antennas[j].frequency) {
                int dx = antennas[j].x - antennas[i].x;
                int dy = antennas[j].y - antennas[i].y;

                // Check for antinode on either side
                for (int k = -1; k <= 1; k += 2) {
                    int antinode_x = antennas[i].x + dx * k;
                    int antinode_y = antennas[i].y + dy * k;

                    // Check if the antinode is in bounds and correctly positioned
                    if (antinode_x >= 0 && antinode_x < cols &&
                        antinode_y >= 0 && antinode_y < rows &&
                        2 * manhattan_distance(antennas[i].x, antennas[i].y, antinode_x, antinode_y) ==
                        manhattan_distance(antennas[i].x, antennas[i].y, antennas[j].x, antennas[j].y)) {
                        antinodes[antinode_y][antinode_x] = 1;
                    }
                }
            }
        }
    }

    // Count unique antinode locations
    int unique_antinode_count = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (antinodes[r][c]) {
                unique_antinode_count++;
            }
        }
    }

    printf("Unique antinode locations: %d\n", unique_antinode_count);

    return 0;
}

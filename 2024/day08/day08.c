#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 8                                  //
// Link: https://adventofcode.com/2024/day/8                  //
////////////////////////////////////////////////////////////////

#define filename "day08input.txt"

#define CHAR_COUNT 62
#define ANT_NUM 10

// Maps character to index (0-61)
int mapCharToIndex(char ch) {
    if (isupper(ch)) return ch - 'A';
    else if (islower(ch)) return (ch - 'a') + 26;
    else if (isdigit(ch)) return (ch - '0') + 52;
    return -1; // Invalid character
}

// Maps index (0-61) back to character
char mapIndexToChar(int index) {
    if (index < 26) return 'A' + index;
    else if (index < 52) return 'a' + (index - 26);
    else if (index < 62) return '0' + (index - 52);
    else return '\0'; // Invalid character
}

// Point struct 
typedef struct {
    int x;
    int y;
} point;

// Frequency struct
typedef struct {
    char freq;
    int length;
    point pos[ANT_NUM]; // Array of points, max length 10 is enough
} frequency;

void fillMatrix(frequency matrix[CHAR_COUNT]) {
    // Iterate over each possible character index (0 to 61)
    for (int i = 0; i < CHAR_COUNT; i++) {
        // Initialize frequency struct for each character
        matrix[i] = (frequency) {mapIndexToChar(i)};
    }
}

// Maps antennae to coordinates in the frequency matrix.
void mapAntennae(frequency mat[CHAR_COUNT], int *rows, int *cols) {
    // Open the file for reading
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Initialize the current position
    int x = 0, y = 0;

    // Iterate over each character in the file
    char ch;
    
    while ((ch = fgetc(fp)) != EOF) {
        // Map the character to an index (0 to 61)
        int index = mapCharToIndex(ch);
        // If the character is valid, store the position in the frequency matrix
        if (index != -1) mat[index].pos[mat[index].length++] = (point) {x, y};

        // Move to the next column
        x++;

        // If we've reached the end of a line, move to the next row
        if (ch == '\n') {
            x = 0;
            y++;
        }
    }

    // Close the file
    fclose(fp);

    // Store the number of rows and columns
    *rows = y + 1;
    *cols = x;
}

// Boolean matrix
typedef struct {
    int rows;
    int cols;
    bool * mat; // Boolean matrix is stored as an array of bools to save space
} boolMatrix;

boolMatrix createBoolMatrix(int rows, int cols) {
    // Allocate memory for the boolean matrix. 
    // The matrix is represented as a 1D array of bools with a size equal to rows * cols.
    bool * mat = calloc(rows * cols, sizeof(bool)); 

    // Initialize each element in the matrix to false (0).
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = false;
    }

    // Return a boolMatrix struct containing the number of rows, columns, and a pointer to the matrix.
    return (boolMatrix) {rows, cols, mat};
}

void freeBoolMatrix(boolMatrix board) {
    free(board.mat);
}

// Function to check if a point is within the bounds of a matrix
bool fitBounds(point a, int rows, int cols) {
    // Check if the x-coordinate is less than 0 or greater than or equal to the number of columns
    if (a.x < 0 || a.x >= cols) return false;

    // Check if the y-coordinate is less than 0 or greater than or equal to the number of rows
    if (a.y < 0 || a.y >= rows) return false;
    
    // If neither of the above conditions are met, the point is within bounds
    return true;
}

// Function to mark the points that makes an antinode for part1, that means they're spaced
// from an antenna and twice the distance from another antenna with the same frequency
void makepoint1(point a, point b, boolMatrix board) {
    // Calculate the difference in x and y coordinates between a and b.
    // This will be used to calculate the coordinates of the point directly above or below a and b.
    // The differents uses the sign to determine the direction.
    int dx = a.x - b.x;
    int dy = a.y - b.y;

    // Calculate the coordinates of the points directly above or below a and b.
    point preA = {a.x + dx, a.y + dy};
    point preB = {b.x - dx, b.y - dy};

    // If the point directly above or below a is within the bounds of the matrix, mark it as visible.
    if (fitBounds(preA, board.rows, board.cols)) {
        board.mat[preA.y * board.cols + preA.x] = true;
    };
    
    // If the point directly above or below b is within the bounds of the matrix, mark it as visible.
    if (fitBounds(preB, board.rows, board.cols)) {
        board.mat[preB.y * board.cols + preB.x] = true;
    };
}

// Function to mark the points that makes an antinode for part2, that means they're on the same line
// as two antennae with the same frequency and they are x times their distance apart from one of the antennae
void makepoint2(point a, point b, boolMatrix board) {
    // Calculate the difference in x and y coordinates between a and b.
    // This will be used to calculate the coordinates of the point directly above or below a and b.
    // The differents uses the sign to determine the direction.
    int dx = a.x - b.x;
    int dy = a.y - b.y;

    // Initialize the boolean variables to check if the points are within bounds
    bool fitA = true, fitB = true;

    // Loop until at least one of the pointer is within the bounds of the matrix
    // meanwhile iterate the distance multiplier
    for (int i = 0; fitA || fitB; i++) {
        // Calculate the coordinates of the points distant i times from a and b
        point preA = {a.x + (dx * i), a.y + (dy * i)};
        point preB = {b.x - (dx * i), b.y - (dy * i)};

        // Check if the points are within the bounds of the matrix
        fitA = fitBounds(preA, board.rows, board.cols);
        fitB = fitBounds(preB, board.rows, board.cols);

        // If the point is within the bounds of the matrix, mark it as visible
        if (fitA) {
            board.mat[preA.y * board.cols + preA.x] = true;
        };
    
        // If the point is within the bounds of the matrix, mark it as visible
        if (fitB) {
            board.mat[preB.y * board.cols + preB.x] = true;
        };
    }
}

// Count the number of points in a given matrix that make an antinode
int countpoints(frequency mat[CHAR_COUNT], int rows, int cols, int part) {
    // Create a boolean matrix to store the points that satisfy the conditions
    boolMatrix board = createBoolMatrix(rows, cols);

    // Iterate over each frequency in the matrix
    for (int i = 0; i < CHAR_COUNT; i++) {
        // Iterate over each position in the frequency
        for (int j = 0; j < mat[i].length; j++) {
            // Iterate over each other position in the frequency after the current one so
            // each pair of positions is checked only once
            for (int k = j + 1; k < mat[i].length; k++) {
                // Check the antinodes using the rules for each part
                if (part == 1) {
                    makepoint1(mat[i].pos[j], mat[i].pos[k], board);
                } else if (part == 2) {
                    makepoint2(mat[i].pos[j], mat[i].pos[k], board);
                }
            }
        }
    }

    // Count the number of points that satisfy the conditions
    int count = 0;
    for (int i = 0; i < rows * cols; i++) {
        if (board.mat[i]) count++;
    }

    // Free the memory allocated for the boolean matrix
    free(board.mat);

    return count;
}
int main() {
    // Declare variables to store the number of rows and columns
    int rows, cols;

    // Declare a frequency array to hold the frequency data for each character
    frequency matrix[CHAR_COUNT];

    // Initialize the frequency matrix with the corresponding character for each index
    fillMatrix(matrix);

    // Map the antennae positions from the input file to the frequency matrix
    // and store the number of rows and columns in the provided variables
    mapAntennae(matrix, &rows, &cols);

    printf("Part 1:\n\tNumber of antinodes that are distant double from an antenna than another of the same frequency: %d\n", countpoints(matrix, rows, cols, 1));
    printf("Part 2:\n\tNumber of antinodes that are on the same line as two antennae with the same frequency: %d\n", countpoints(matrix, rows, cols, 2));

    // Return 0 to indicate successful execution
    return 0;
}

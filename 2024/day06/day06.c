#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 6                                  //
// Link: https://adventofcode.com/2024/day/6                  //
////////////////////////////////////////////////////////////////

#define filename "day06input.txt"

// Define the various directions 
enum direction { UP = 3, LEFT = 5, DOWN = 7, RIGHT = 11 };

// Free the dynamically allocated matrix of characters
void freeMatrix(char ** mat, int rows) {
    if (mat == NULL) return;

    // Iterate over each row of the matrix
    for (int i = 0; i < rows; i++) {
        if (mat[i] != NULL) {
            free(mat[i]); 
        }
    }

    free(mat);
}


// Frees a dynamically allocated matrix of integers
void freeIntMatrix(int ** mat, int rows) {
    if (mat == NULL) return;

    // Iterate over each row of the matrix
    for (int i = 0; i < rows; i++) {
        if (mat[i] != NULL) {
            free(mat[i]); 
        }
    }

    // Free the memory allocated for the array of row pointers
    free(mat);
}


// Clean up the matrix after it's been used
void cleanMatrix(char ** mat, int rows, int cols, int ogx, int ogy) {
    // Iterate over every cell in the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // If the cell is not a wall, set it to be a floor
            if (mat[i][j] != '#') {
                mat[i][j] = '.';
            }
        }
    }

    // Put the guard back in it's original position
    mat[ogx][ogy] = '^';
}


// Get the dimensions of the file
void getFileDimensions(int *rows, int *cols) {
    // Open the file
    FILE *file = fopen(filename, "r");  
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }

    *rows = 1;
    *cols = 0;
    char ch;

    // Read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        // Count the number of columns for the first row only
        if (*rows == 1 && ch != '\n') {
            *cols += 1;
        }

        // Count the number of newlines
        if (ch == '\n') {
            *rows += 1;
        }      
    }

    fclose(file);  // Close the file
}

// Get the matrix from the file
char ** getMatrix(int rows, int cols) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // Allocate the matrix to the number of rows
    char ** mat = calloc(rows, sizeof(char *));

    // Read the file line by line and store it in the matrix
    for (int i = 0; i < rows; i++) {
        mat[i] = calloc(cols, sizeof(char));
        fscanf(fp, "%s\n", mat[i]);
    }

    return mat;
}


// Search the guard initial position in the array
void getGuardPos(char ** mat, int rows, int cols, int * guardx, int * guardy) {
    // Iterate over every cell in the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // If the cell is the guard return it
            if (mat[i][j] == '^') {
                *guardx = i;
                *guardy = j;
                return;
            }
        }
    }
}

// Rotate the direction the guard is facing
int rotate(int guard, int *dir) {
    // Rotate the direction
    if (dir[0] == 0) {
        // If the direction is up or down, rotate the direction left or right
        dir[0] = dir[1];
        dir[1] = 0;
    } else {
        // If the direction is left or right, rotate the direction up or down
        dir[1] = -dir[0];
        dir[0] = 0;
    };

    // Return the new direction
    switch (guard) {
        case UP: return RIGHT;
        case RIGHT: return DOWN;
        case DOWN: return LEFT;
        case LEFT: return UP;
    }
}

// Check if the position is in the bounds of the matrix
int inBounds(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Get the char representing the direction the guard is facing
char charDirection(int guard) {
    switch (guard) {
        case UP: return '^';
        case RIGHT: return '>';
        case DOWN: return 'v';
        case LEFT: return '<';
    }
}

// Move the guard and return the number of tiles visited
int guardRoute(char ** mat, int rows, int cols) {
    int x, y;
    // Get the initial position of the guard
    getGuardPos(mat, rows, cols, &x, &y);

    // Store the original position of the guard
    int ogx = x, ogy = y;

    // Initialize the count of tiles visited
    int count = 1;

    // Define the initial direction vector (up) and integer
    int dir[2] = {-1, 0};
    int guard = UP;

    // Continue moving the guard while it is within the matrix bounds
    while (inBounds(x, y, rows, cols)) {
        // Mark the current position of the guard with 'X'
        mat[x][y] = 'X';

        // Break if the next position is out of bounds
        if (!inBounds(x + dir[0], y + dir[1], rows, cols)) break;

        // Rotate the guard if the next position is a wall
        if (mat[x + dir[0]][y + dir[1]] == '#') guard = rotate(guard, dir);
        
        // Increment the step count if the next position is a floor he hasn't visited yet
        if (mat[x + dir[0]][y + dir[1]] == '.') count++;

        // Move the guard to the next position
        x += dir[0];
        y += dir[1];

        // Draw the current position of the guard in the matrix
        mat[x][y] = charDirection(guard);
    }
    
    // Clean up the matrix and reset the guard to the original position
    cleanMatrix(mat, rows, cols, ogx, ogy);

    // Return the count of steps the guard took
    return count;
}

// Initialize a matrix of ones
int ** zeroMatrix(int rows, int cols) {
    // Allocate the matrix to the number of rows
    int ** mat = calloc(rows, sizeof(int *));

    // For every row in the matrix allocate the number of columns
    for (int i = 0; i < rows; i++) {
        mat[i] = calloc(cols, sizeof(int));
        // Set every cell to 1
        for (int j = 0; j < cols; j++) {
            mat[i][j] = 1;
        }
    }

    return mat;
}


// Check if the guard will get stuck in an infinite loop
int guardRouteLooping(char ** mat, int rows, int cols, int x, int y) {
    // Create a matrix to keep track of which squares the guard has already visited
    int ** mat2 = zeroMatrix(rows, cols);

    // Initialize the guard's direction and position
    int guard = UP;
    int dir[2] = {-1, 0};

    // Initialize a flag to check if the guard will get stuck in an infinite loop
    int check = 0;

    // Loop until the guard is out of bounds of the matrix
    while (inBounds(x, y, rows, cols)) {
        // Check if the guard has already visited this square before by checking if it's a multiple of the
        // prime number representing the direction
        if (mat2[x][y] % guard == 0) {
            // If yes, then the guard will get stuck in an infinite loop
            check = 1;
            break;
        }

        // Mark the current square as visited by multiplying it by the direction represented by a prime number
        mat2[x][y] *= guard;

        // Check if the next square is out of bounds
        if (!inBounds(x + dir[0], y + dir[1], rows, cols)) break;

        // Check if the next square is a wall or an obstacle
        if (mat[x + dir[0]][y + dir[1]] == '#' || mat[x + dir[0]][y + dir[1]] == 'O') {
            // If yes, then rotate the guard's direction and continue to the next iteration
            // to avoid crusing into a second wall if there is one
            guard = rotate(guard, dir);
            continue;
        };

        // Move the guard to the next square
        x += dir[0];
        y += dir[1];
    }

    // Free the memory allocated for the matrix of integers
    freeIntMatrix(mat2, rows);
    return check;
}

// Return the number of positions you can add an obstacle to block the guard
int blockGuard(char ** mat, int rows, int cols) {
    // Initialize a counter for the number of positions that block the guard
    int count = 0;

    // Variables to store the initial position of the guard
    int x, y;
    // Get the guard's initial position in the matrix
    getGuardPos(mat, rows, cols, &x, &y);

    // Iterate over each cell in the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Skip cells that have either a wall or the guard
            if (mat[i][j] != '.') continue;
            
            // Temporarily set the current empty cell to be a wall
            mat[i][j] = 'O';

            // Check if placing a wall here causes the guard to loop infinitely
            // Increment the count if the guard would get stuck
            if (guardRouteLooping(mat, rows, cols, x, y)) count++;
            
            // Reset the cell back to an empty space
            mat[i][j] = '.';
        }
    }

    // Return the total count of obstacle positions that would block the guard
    return count;
}

int main(void) {
    // Get the dimensions of the matrix from the file
    int rows, cols;
    getFileDimensions(&rows, &cols);

    // Read the matrix from the file
    char ** mat = getMatrix(rows, cols);

    // Part 1: Find the total number of squares visited by the guard
    int count = guardRoute(mat, rows, cols);
    printf("Part 1: \n\tTotal squares visited by the guard: %d\n", count);

    // Part 2: Find the total number of positions that an obstacle can be placed to block the guard
    count = blockGuard(mat, rows, cols);
    printf("Part 2: \n\tTotal position of obstacles to block the guard: %d\n", count);

    // Free the memory allocated for the matrix of characters
    freeMatrix(mat, rows);
}

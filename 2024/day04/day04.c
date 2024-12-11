#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 4
// Link: <a href="...">https://adventofcode.com/2024/day/4</a>
////////////////////////////////////////////////////////////////

#define filename "day04input.txt"

// Get the dimensions of the file
// This function takes two pointers, *rows and *cols, and fills them in with the number of rows and columns in the file respectively
void getFileDimensions(int *rows, int *cols) {
    // Open the file
    FILE *file = fopen(filename, "r");  
    if (file == NULL) {
        printf("Error while opening the file.\n");
        // If the file can't be opened, exit the program
        exit(1);
    }

    *rows = 1;
    *cols = 0;
    char ch;

    // Read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        // Count the number of columns for the first row only
        if (*rows == 1 && ch != '\n') {
            // Only count the number of columns on the first row
            *cols += 1;
        }

        // Count the number of rows by counting the newlines
        if (ch == '\n') {
            *rows += 1;
        }      
    }

    fclose(file);
}


// Get the matrix from the file
char ** getMatrix(int rows, int cols) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return NULL;
    }

    // Allocate the matrix to the number of rows
    char ** mat = calloc(rows, sizeof(char *));

    // Read the file line by line and store it in the matrix
    for (int i = 0; i < rows; i++) {
        mat[i] = calloc(cols, sizeof(char));
        fscanf(fp, "%s\n", mat[i]);
    }

    fclose(fp);

    return mat;
}

// Check if the direction is valid
int checkDirection(char ** mat, int r, int c, int dirx, int diry) {
    char * xmas = "XMAS";

    // Check if every char in the direction is part of XMAS
    for (int i = 0; i < 4; i++) {
        if (mat[r + dirx * i][c + diry * i] != xmas[i]) return 0;
    }

    return 1;
}

// Count the number of XMAS appearing in the matrix
int countXmas(char ** mat, int rows, int cols) {

    // The 8 possible directions to check
    int dirs[8][2] = {
        { -1, -1 }, { -1,  0 }, { -1,  1 },  //  ↖ ↑ ↗
        {  0, -1 },             {  0,  1 },  //  ← ch →
        {  1, -1 }, {  1,  0 }, {  1,  1 },  //  ↙ ↓ ↘
    };

    int count = 0;

    // Check every elem in the matrix
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Only X can start an XMAS
            if (mat[r][c] != 'X') continue;
            
            // Check the 8 directions
            for (int i = 0; i < 8; i++) {
                // Check if the direction fits the boundaries of the matrix
                if (r + dirs[i][0] * 3 >= rows || c + dirs[i][1] * 3 >= cols) continue;
                if (r + dirs[i][0] * 3 < 0 || c + dirs[i][1] * 3 < 0) continue; 
                
                // Increase the count if the direction is valid
                count += (checkDirection(mat, r, c, dirs[i][0], dirs[i][1]));
            }
        }
    }

    return count;
}

// Check if the direction forms the half of a cross
int isCross(char t, char b) {
    // Checks if the two params are parts of the line MAS
    if (t == 'M' && b == 'S') return 1;
    if (b == 'M' && t == 'S') return 1;

    return 0;
}

int countMas(char ** mat, int rows, int cols) {
    int count = 0;

    // Check every elem in the matrix
    for (int r = 1; r < rows - 1; r++) {
        for (int c = 1; c < cols - 1; c++) {
            // Only A can be the center of a cross
            if (mat[r][c] != 'A') continue;

            // Skip to the next elem if one of the line isn't part of the cross
            if (!isCross(mat[r - 1][c - 1], mat[r + 1][c + 1])) continue;
            if (!isCross(mat[r - 1][c + 1], mat[r + 1][c - 1])) continue;
            
            count ++;
        }
    }

    return count;
}

// Free the dynamically allocated matrix
void freeMatrix(char ** mat, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }

    free(mat);
} 

int main(void) {
    int rows, cols;
    getFileDimensions(&rows, &cols);
    char ** mat = getMatrix(rows, cols);

    // Part 1: Count the number of XMAS in every of the 8 directions
    printf("Part 1: \n\tTotal occurrences of XMAS: %d\n", countXmas(mat, rows, cols));
    
    // Part 2: Count the number of crosses made by MAS
    printf("Part 2: \n\tTotal occurrences of X-MAS: %d\n", countMas(mat, rows, cols));

    // Free the dynamically allocated matrix
    freeMatrix(mat, rows);

    return 0;
}

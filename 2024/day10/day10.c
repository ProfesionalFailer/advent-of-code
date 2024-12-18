#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 10                                 //
// Link: https://adventofcode.com/2024/day/10                 //
////////////////////////////////////////////////////////////////

#define filename "day10input.txt"

// Define macro to switch position between 2d array and 1d array
#define ARR_POS(x, y, cols) x + y * cols // 2d to 1d
#define CORDS(x, cols) x % cols, x / cols // 1d to 2d

// Define a struct to represent a hiketrail
typedef struct {
    uint8_t rows;
    uint8_t cols;
    uint8_t * mat; // Using a 1d array to save space and improve performance 
} hiketrail;

hiketrail readFile(void) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        // If the file can't be opened, exit the program
        exit(1);
    }

    // Read the first line of the file, and store the number of columns
    char ch;
    int cols = 0;
    while ((ch = fgetc(fp)) != '\n') cols++;

    // Move the file pointer to the end of the file
    fseek(fp, 0, SEEK_END);

    // Calculate the number of rows
    int rows = (ftell(fp) - 1) / (cols + 1);
    // - 1 to account for the offset
    // + 1 to account for the newline character

    // Reset the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // Allocate memory for the matrix as an array of 8 bit unsigned integers
    // to save space
    uint8_t * mat = calloc(rows * cols, sizeof(uint8_t));

    // Read the matrix character by character
    int i = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') continue;
        // Store the value as an 8 bit unsigned integer
        mat[i++] = ch - '0';
    }

    // Close the file
    fclose(fp);

    // Return the struct containing the matrix, rows and columns
    return (hiketrail) { rows, cols, mat };
}


// Struct to store the coordinates of the destinations
// using a 16 bit integer as it's uknown how many destinations there will be
// 65535 should be more than enough while 256 could be too small
typedef struct {
    uint16_t len;
    uint16_t *pos;
} destinations;

// Add a coordinate to the destinations array if it's not already there.
void addCords(destinations dests, uint16_t x) {
    // Iterate over the array of coordinates and check if the coordinate 
    for (uint16_t i = 0; i < dests.len; i++) {
        // If the coordinate is already in the array, return. Using +1 as 0 is the empty space
        if (dests.pos[i] == x + 1) return;
        else if (dests.pos[i] == 0) {
        // Insert at the first empty slot
            dests.pos[i] = x + 1;
            return;
        };
    }
}

// Recursively make a trail from a given coordinate and add the coordinates to the destinations array
void makeTrail(hiketrail trail, uint8_t x, uint8_t y, destinations dests) {
    // Get the value at the current coordinate
    uint8_t val = trail.mat[ARR_POS(x, y, trail.cols)];
    
    // Base case if end of trail is reached
    if (val == 9) {
        // Add the coordinate to the destinations array
        addCords(dests, (uint16_t) ARR_POS(x, y, trail.cols));
        return;
    };

    // Get the next coordinates as signed ints to account for negative coordinates
    // that are out of bounds
    int16_t next[4][2] = {
        { x + 1, y },
        { x - 1, y },
        { x, y + 1 },
        { x, y - 1 }
    };

    // Iterate over the array of next coordinates
    for (uint8_t i = 0; i < 4; i++) {
        // Check if the next coordinate is out of bounds
        if (next[i][0] < 0 || next[i][0] >= trail.cols) continue;
        if (next[i][1] < 0 || next[i][1] >= trail.rows) continue;

        // Check if the next coordinate succesfully continues the trail
        if (trail.mat[(uint16_t) ARR_POS(next[i][0], next[i][1], trail.cols)] != val + 1) continue;

        // Recursively make a trail to the next coordinate
        makeTrail(trail, (uint8_t) next[i][0], (uint8_t) next[i][1], dests);
    }
}

// Make a trail from a given coordinate and return the number of destinations
uint16_t startTrail(hiketrail trail, uint16_t start, destinations dests) {
    // Reset the destinations array
    for (uint16_t i = 0; i < dests.len; i++) {
        dests.pos[i] = 0;
    }

    makeTrail(trail, CORDS(start, trail.cols), dests);

    // Count the number of destinations
    uint16_t count = 0;
    for (uint16_t i = 0; i < dests.len; i++) {
        if (dests.pos[i] == 0) break;
        count++;
    }

    return count;
}

// Count the sum of the number of destinations reachable from each starting point
uint16_t countTrails(hiketrail trail) {
    uint16_t des_count = 0;

    // Count the number of destinations
    for (uint16_t i = 0; i < trail.rows * trail.cols; i++) {
        if (trail.mat[i] == 9) des_count++;
    }

    // Allocate memory for the destinations array and initialize it in the struct
    destinations dests = (destinations) {des_count, calloc(des_count, sizeof(uint16_t))};

    // For every character in the trail
    uint16_t count = 0;
    for (uint16_t i = 0; i < trail.rows * trail.cols; i++) {
        // If it's a zero, start a new trail
        if (trail.mat[i] == 0) {
            count += startTrail(trail, i, dests);
        };
    }

    free(dests.pos);

    return count;
}

// Recursively make a trail from a given coordinate
uint16_t makeTrail2(hiketrail trail, uint8_t x, uint8_t y) {
    // Get the value at the current coordinate
    uint8_t val = trail.mat[ARR_POS(x, y, trail.cols)];

    // Base case if end of trail is reached
    if (val == 9) return 1;

    // Count is the number of trails that can be completed
    // that include the current coordinate
    uint16_t count = 0;

    // Get the next coordinates as signed ints to account for negative coordinates
    int16_t next[4][2] = {
        { x + 1, y },
        { x - 1, y },
        { x, y + 1 },
        { x, y - 1 }
    };

    // Iterate over the array of next coordinates
    for (uint8_t i = 0; i < 4; i++) {
        // Check if the next coordinate is out of bounds
        if (next[i][0] < 0 || next[i][0] >= trail.cols) continue;
        if (next[i][1] < 0 || next[i][1] >= trail.rows) continue;

        // Check if the next coordinate succesfully continues the trail
        if (trail.mat[(uint16_t) ARR_POS(next[i][0], next[i][1], trail.cols)] != val + 1) continue;

        // Add the number of possible trails that include both the current and next coordinate
        count += makeTrail2(trail, (uint8_t) next[i][0], (uint8_t) next[i][1]);
    }

    return count;
}

// Count the number of possible trails
uint16_t countTrails2(hiketrail trail) {
    uint16_t count = 0;

    // For every character in the trail
    for (uint16_t i = 0; i < trail.rows * trail.cols; i++) {
        // If it's a zero, start a new trail
        if (trail.mat[i] == 0) {
            count += makeTrail2(trail, CORDS(i, trail.cols));
        };
    }

    return count;
}

int main(void) {
    hiketrail trail = readFile();

    printf("Part 1:\n\tSum of the number of destinations from each low point: %u\n", countTrails(trail));
    printf("Part 1:\n\tNumber of possible trails: %u\n", countTrails2(trail));

    return 0;
}

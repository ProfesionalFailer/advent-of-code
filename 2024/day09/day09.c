#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 9                                  //
// Link: https://adventofcode.com/2024/day/9                  //
////////////////////////////////////////////////////////////////

#define filename "day09input.txt"

// Reads the contents of a file in a passed array and returns the number of bytes read.
int readFile(char ** buffer) {
    FILE * fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return -1;
    }

    // Get the size of the file by seeking to the end of the file and calculating the offset
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);

    // Reset the file pointer to the beginning of the file
    rewind(fp);

    // Allocate memory to store the contents of the file
    *buffer = malloc(size + 1);

    // Read the contents of the file into the buffer
    fread(*buffer, 1, size, fp);

    // Add a null character at the end of the buffer so that it can be used as a string
    (*buffer)[size] = '\0';

    fclose(fp);

    return size;
}

// Calculate the new size of the disk by summing up the length of the commands
int getNewSize(char * buffer, int size) {
    int new_size = 0;
    
    // Iterate through the buffer until a newline character is found
    for (int i = 0; i < size; i++) {
        // If a newline character is found, break out of the loop as the input is a oneliner
        if (buffer[i] == '\n') break;
        
        // Add the value of the current character to the new size as the digit could either mean x numbers or x spaces
        // The value of the character is calculated by subtracting the ASCII value of '0' from the character
        // This is because the characters in the buffer are the ASCII representations of the numbers
        new_size += (int) buffer[i] - '0';
    }
    
    return new_size;
}

//Reads the contents of the file into a buffer and then processes the commands in the buffer to create a disk.
// The disk is represented as a 1D array of integers, spaces are represented as -1 and numbers are repsented
// by their id
int * makeDisk(int * newsize) {
    // Read the contents of the file into a buffer
    char * buffer;
    int size = readFile(&buffer);

    // Calculate the new size of the disk
    *newsize = getNewSize(buffer, size);

    // Allocate memory to store the disk
    int * disk = calloc(*newsize, sizeof(int));

    // Initialize variables to keep track of the current position in the disk and the current id
    int id = 0, j = 0;

    // Iterate through the buffer
    for (int i = 0; i < size; i++) {
        // If a newline character is found, break out of the loop as the input is a oneliner
        if (buffer[i] == '\n' || buffer[i] == '\0') break;

        // Get the number of positions to fill from the buffer
        int count = buffer[i] - '0';

        // Get the value to fill the positions with from the buffer
        int val = (i % 2 == 0) ? id++ : -1;

        // Fill the positions in the disk with the value
        // Memset fills count * sizeof(int) bytes with the value val starting from the position j
        for (int _ = 0; _ < count; _++) {
            disk[j++] = val;
        }

        //memset(disk + j, val, count * sizeof(int));
    }

    // Free the memory allocated for the buffer
    free(buffer);

    // Return the pointer to the first element of the disk array
    return disk;
}


// This function takes a disk represented as a 1D array of integers and compacts it by shifting all integers from right
// to the first available space from the left
void compactDisk(int * disk, int new_size) {
    // It uses two pointers to keep track of the current position in the disk
    // one for reading and one for writing
    int write = 0, read = new_size - 1;

    // Loop until the read pointer reaches the end of the disk
    while (write < read) {
        // Move the pointer until read is a number
        // and write is on a space
        if (disk[read] == -1) read--;
        else if (disk[write] != -1) write++;
        else {
            // Swap the values on the right and left pointers
            disk[write] = disk[read];
            disk[read] = -1;
        }
    }
}



// This function calculates a checksum for a given disk.
uint64_t checkSum(int * disk, int new_size) {
    uint64_t sum = 0;

    // Loop through each element in the disk
    for (int i = 0; i < new_size; i++) {
        // If the current element is a space (-1), skip it
        if (disk[i] == -1) continue;
        
        // Multiply the id of the current element by its index and add it to the sum
        sum += disk[i] * i;
    }

    // Return the checksum
    return sum;
}


// This function calculates the number of consecutive numbers of the same id from right to left starting from a given position
int countNums(int * disk, int start) {
    int count = 0;
    int num = disk[start];

    // Start from the given position and move to the left
    for (int i = start; i >= 0; i--) {
        // If the current element has the same id as the element at the given position increment the counter
        if (disk[i] == num)  count++;
        else break;
        // Break out of the loop if the current element has a different id
    }

    return count;
}

// Finds the first gap from the left that fits the given suze
int findGap(int * disk, int start, int size) {
    // Initialize a counter for tracking the current gap size
    int gap = 0;

    // Iterate through the disk array from the beginning up to the 'start' index
    for (int i = 0; i < start; i++) {
        // Check if the current element is a space (-1)
        if (disk[i] == -1) gap++;
        else gap = 0; // If the current element is not a space, gap has ended

        // If the current gap can fit the given size, return the starting index
        if (gap >= size) return i - gap + 1;
    }

    // Return -1 if no suitable gap is found
    return -1;
}

// Compacts the disk by moving all the blocks of the same id to the leftmost part of the disk
// in the order of their id.
void compactDiskByFile (int * disk, int size) {
    // Initialize two pointers, write and read.
    int write = 0, read = size - 1;
    
    // Initialize a counter, num_read, to keep track of the number of consecutive blocks of the same id
    int num_read = 0;

    // Loop through the disk from right to left
    while (read >= 0) {
        // If the current block is a space (-1), move the read to the left
        if (disk[read] == -1) {
            read--;
            continue;
        }

        // Count the number of consecutive blocks of the same id starting from the current block
        num_read = countNums(disk, read);

        // Find the first gap that can fit the given number of blocks
        write = findGap(disk, read, num_read);

        // If no gap is found, move the read pointer to the left of the current block of numbers and continue
        if (write == -1) {
            read -= num_read;
            continue;
        }

        // Move the blocks from their position to the gap, one by one
        for (int i = 0; i < num_read; i++) {
            disk[write++] = disk[read];
            disk[read--] = -1;
        }
    }
}


int main() {
    // Read the disk from the file and allocate memory for it
    int size;
    int * disk = makeDisk(&size);

    // Create a copy of the disk for part 2
    int * disk2 = calloc(size, sizeof(int));
    memcpy(disk2, disk, size * sizeof(int));

    // Execute part 1
    compactDisk(disk, size);
    printf("Part 1: \n\tCheck sum: %llu\n", checkSum(disk, size));
    free(disk);

    // Execute part 2
    compactDiskByFile(disk2, size);
    printf("Part 2: \n\tCheck sum: %llu\n", checkSum(disk2, size));
    free(disk2);

    // Return 0 to indicate successful execution
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 7                                  //
// Link: https://adventofcode.com/2024/day/7                  //
////////////////////////////////////////////////////////////////

#define filename "day07input.txt"

// Count the number of integers in a given string
// Returns the number of integers found
int countInts(char * str) {
    int count = 0; // Initialize the count of integers to 0

    int digPrev = 1; 
    // Last digits signals whether the last character analyzed was a digit to check for the start of a new integer
    // 0 = digit, 1 = non-digit. It's initialized to 1 to account strings starting with a digit
    
    int size = strlen(str); // Get the size of the string

    for (int i = 0; i < size; i++) {
        if (isdigit(str[i]) && digPrev) { // If the current character is a digit and the last one wasn't
            count++; // Increment the count signaling a new integer was found
        }

        digPrev = isdigit(str[i]) ? 0 : 1; // Update the last digit variable
    }

    return count;
}

// Checks if a given array of integers can be combined in a way that equals a given
// number using only additions and multiplications.
int isSolvable2Ops(int * arr, int len, uint64_t sol) {
    int count = 0;
    // Calculate the number of possible combinations of operators between the elements of the array.
    // len - 1 because between every two elements there is only one operator
    uint16_t chances = 1 << (len - 1);

    // Loop through each possible combination of the operators
    for (uint16_t i = 0; i < chances; i++) {
        uint64_t total = arr[0];
        // Loops through the array except the first element
        for (int j = 0; j < len - 1; j++) {
            // Check if the jth bit of the current combination is 1 or 0 and run the corresponding operation
            // Using chances assures every possible combination is checked because all numbers have a unique 
            // binary representation.
            total = ((i >> j) & 1) ? total + arr[j + 1] : total * arr[j + 1];
        }

        // If the total equals the solution, return 1
        if (total == sol) return 1;
    }

    // If no combination was found, return 0
    return 0;
}


// Converts a given number to its base-3 representation
// and stores the result in the provided array
void toBase3(int num, int len, int ** arr) {
    // Iterate over each position in the array
    for (int i = 0; i < len; i++) {
        // Divide by 3 and add the remainder to the array
        (*arr)[i] = num % 3;
        num /= 3;
    }
}

// Calculate 3 raised to the power of n
int pow3(int n) {
    int res = 1; // Initialize result to 1, as any number to the power of 0 is 1

    // Loop from 0 to n-1
    for (int i = 0; i < n; i++) {
        // Multiply the result by 3 in each iteration
        res *= 3;
    }

    // Return the final result
    return res;
}

// Concatenates n1 and n2 by shifting n1 to the left for every digit of n2
uint64_t concat(uint64_t n1, int n2) {
    // Calculate the number of digits in n.
    int digits = log10(n2) + 1;
    
    
    // Loop through each digit in n and multiply the result by 10. This effectively
    // shifts the digit to the left.
    for (int i = 0; i < digits; i++) n1 *= 10;
    
    // Return the final result
    return n1  + n2;
}

// Checks if a given array of integers can be combined in a way that equals a given
// number using only additions, multiplications and concatenation.
int isSolvable3Ops(int * arr, int len, uint64_t sol) {
    int count = 0;

    // Calculate the number of possible combinations of operators between the elements of the array.
    //  len - 1 because between every two elements there is only one operator
    uint32_t chances = pow3(len - 1);

    // Allocate an array to store the operators for each combination
    int * ops = calloc(len - 1, sizeof(int));

    // Loop through each possible combination of the operators
    for (uint32_t i = 0; i < chances; i++) {
        uint64_t total = arr[0];

        // Convert the current combination to its base-3 representation and
        // store it in the ops array
        toBase3(i, len - 1, &ops);
        
        // Loop through the array except the first element
        for (int j = 0; j < len - 1; j++) {
            // Check if the jth bit of the current combination is 0, 1 or 2 and run the corresponding operation
            // Using ops assures every possible combination is checked because all numbers have a unique 
            // trinary representation.
            if (ops[j] == 0) {
                total += arr[j + 1];
            } else if (ops[j] == 1) {
                total *= arr[j + 1];
            } else {
                total *= lshift(arr[j + 1]);
                total += arr[j + 1];
            }
        }
        
        // If the total equals the solution, return 1
        if (total == sol) return 1;
    }

    // If no combination was found, return 0
    return 0;
}



// Reads a file line by line, parses each line into two parts: a number and an array of numbers.
// Then, it sums the number at the beginning of the line if the array of numbers can be combined 
// to equal that value using only two or three operators (depending on the value of the part parameter).
uint64_t parseFile(int part) {
    uint64_t count = 0;
        
    // Open the file to read from
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return 1;
    }

    char line[256];
    
    // For each line in the file
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Count the number of integers in the line and allocate an array to store them
        int len = countInts(line) - 1;
        int * tmp = calloc(len, sizeof(int));   
        
        // Read the number at the beginning of the line
        uint64_t sol;
        sscanf(line, "%llu", &sol);

        // Skip the colon and the space after it
        char * ptr = line;
        while (*ptr != ':') ptr++;
        ptr++;
        
        // For each number in the array
        for (int i = 0; i < len; i++) {
            // Read the number from the line
            sscanf(ptr, "%d", &(tmp[i]));
            
            // Move the pointer past the number just read
            ptr++;
            while (*ptr != ' ') ptr++;
        }

        // Check if the array can be combined in a way that equals the number at the beginning of the line
        if (part == 1) {
            if (isSolvable2Ops(tmp, len, sol)) count+= sol;
        } else {
            if (isSolvable3Ops(tmp, len, sol)) count+= sol;
        }

        // Free the memory allocated for the array
        free(tmp);
    }

    return count;
    
}



int main(void) {
    printf("Part 1: \n\tNumber of solvable equations with operators (+, *): %llu\n", parseFile(1));
    printf("Part 2: \n\tNumber of solvable equations with operators (+, *, ||): %llu\n", parseFile(2));

    return 0;
}
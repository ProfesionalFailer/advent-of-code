#include <stdio.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 1
// Link: <a href="...">https://adventofcode.com/2024/day/1</a>
////////////////////////////////////////////////////////////////

#define filename "day01input.txt"

// Reads two arrays from a file and returns the number of elements in the arrays
int readArrays(int ** arrA, int ** arrB) {
    // Opens the file in read mode
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return 1;
    }   

    // Initialize the number of elements and the variables to store 
    //the two integers on each line of the file
    int n = 0;
    int a, b;

    // Read all the numbers from the file till the end
    while (fscanf(fp, "%d %d", &a, &b) == 2) {
        // Increment the number of elements in the arrays
        n++;

        // Reallocate the arrays to have one more element
        *arrA = (int *)realloc(*arrA, n * sizeof(int));
        *arrB = (int *)realloc(*arrB, n * sizeof(int));

        // Add the new values on that line to the arrays
        (*arrA)[n - 1] = a;
        (*arrB)[n - 1] = b;
    }

    // Return the number of elements in the arrays
    return n;
}



// Swaps the pointers to two integers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Returns the distance between two arrays
int distance(int *a, int *b, int n) {
    int d = 0;
    for (int i = 0; i < n; i++) {
        // Adds the distance between the two elements of the same index in the two arrays
        d += abs(a[i] - b[i]);
    }
    return d;
}

// Sorts an array using the bubble sort algorithm
void bubblesort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // If the current element is greater than the next one, swap them
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Returns the number of occurrences of a target value in an array
int countOccurrences(int *arr, int n, int target) {
    // Initialize a counter to keep track of the number of occurrences
    int count = 0;

    // Loop through each element in the array
    for (int i = 0; i < n; i++) {
        // If the current element is equal to the target value
        if (arr[i] == target) {
            // Increment the counter
            count++;
        }
    }

    // Return the number of occurrences
    return count;
}

// Returns an array with the product between each element of a and the number of occurrences of that element in b
int * arrProduct(int *a, int *b, int n) {
    // Allocate memory for the new array
    int * c = (int *)malloc(n * sizeof(int));

    // For each element in a
    for (int i = 0; i < n; i++) {
        // Multiply the element by the number of times it appears in b
        c[i] = a[i] * countOccurrences(b, n, a[i]);
    }

    return c;
}


// Returns the sum of the values in an array
int arrSum(int *arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int main(void) {
    int *a  = NULL, *b  = NULL, *c = NULL;
    int n = readArrays(&a, &b);

    // Sort the arrays
    bubblesort(a, n);
    bubblesort(b, n);

    // Part 1: Print the distance between the two arrays
    printf("Part 1: \n\tDistance between a and b is: %d\n", distance(a, b, n));

    c = arrProduct(a, b, n);

    // Part 2: Print the sum of the product between the two arrays
    printf("Part 2: \n\tSum of arrC is: %d\n", arrSum(c, n));

    // Free the memory and close the file
    free(a); free(b); free(c);
    
    return 0;
}
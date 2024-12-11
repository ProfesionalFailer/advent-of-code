#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 2
// Link: <a href="...">https://adventofcode.com/2024/day/2</a>
////////////////////////////////////////////////////////////////

#define filename "day02input.txt"

// Reads a line from a file and returns the number of integers it contains
int readLine(FILE *fp, int **arr) {
    int tmp, n = 0;
    char ch;

    int *tempArr = NULL;

    // Read till the end of the line or the end of the file
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        // Skip to the next character if it's not a digit
        if (!isdigit(ch)) continue;

        tmp = 0;

        // Build the integer from left to right
        do {
            tmp = tmp * 10 + (ch - '0');
            ch = fgetc(fp);
        } while (isdigit(ch));

        // Reallocate the array and add the new value
        tempArr = (int *)realloc(tempArr, (n + 1) * sizeof(int));
        if (tempArr == NULL) {
            printf("Memory allocation failed\n");
            free(tempArr);
            return 0;
        }

        tempArr[n] = tmp;
        n++;

        // Break the loop if the end of the line is reached, necessary since the last character hasn't been read yet
        if (ch == '\n') break;
    }

    // Save the array on the pointer passed and return the number of elements
    *arr = tempArr;

    return n;
}

// Checks if an array is increasing (strictly or not)
int isIncreasing(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Checks if an array is decreasing (strictly or not)
int isDecreasing(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Checks if an array is safe
int isSafe(int *arr, int n) {
    // Check if the array is either increasing or decreasing
    if (!isIncreasing(arr, n) && !isDecreasing(arr, n)) {
        return 0;    
    }

    // Check if the distance between every element and the consequent is in the range [1, 3]
    int tmp;
    for (int i = 0; i < n - 1; i++) {
        tmp = abs(arr[i] - arr[i + 1]);
        if (tmp < 1 || tmp > 3) {
            return 0;
        }
    }

    return 1;
}

int countSafeReports(void) {
    // Open the file
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return 1;    
    }

    // Allocate memory for the array that will be read from the file
    int * tmp;
    int count = 0, n;

    // Read the file line by line, stop when the end of the file is reached
    while ((n = readLine(fp, &tmp)) > 0) {

        // Check if the line is safe
        if (isSafe(tmp, n)) {
            // If the line is safe, increment the count
            count++;
        }

        // Free the memory allocated for the array
        free(tmp);
    }

    fclose(fp);

    return count;
}


// Generate a subarray excluding the element at the specified index passsed
void subAray(int *arr, int n, int index, int ** tmp) {
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i == index) continue;
        (*tmp) [j++] = arr[i];
    }
}

int countUnsafeReports(void) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return 1;    
    }

    // Allocate memory for the array that will be read from the file
    int *tmp, *tmp2;
    int count = 0, n = 0, k = 0, check = 0;

    // Read the file line by line, stop when the end of the file is reached
    while ((n = readLine(fp, &tmp)) > 0) {

        // Check if the line is safe
        if (isSafe(tmp, n)) {free(tmp); continue; }

        // If the array wasn't safe, check if removing any layer makes it safe
        tmp2 = (int *) malloc((n - 1) * sizeof(int));
        for (int i = 0; i < n; i++) {
            // Check if removing the layer at the current index makes the array safe
            subAray(tmp, n, i, &tmp2);
            if (isSafe(tmp2, n - 1)) { count++; break; }
        }

        // Free the memory allocated for the array
        free(tmp);
        free(tmp2);
    }

    // Close the file
    fclose(fp);

    return count;

}
int main(void) {

    // Part 1: Count the number of safe reports
    int safeCount = countSafeReports();
    printf("Part 1: \n\tNumber of safe reports: %d\n", safeCount);

    // Part 2: Count the number of safe reports and unsafe reports that could be fixed by removing a layer
    printf("Part 2: \n\tNumber of safe reports with a single bad level tolerance: %d\n", safeCount + countUnsafeReports());

    return 0;
}

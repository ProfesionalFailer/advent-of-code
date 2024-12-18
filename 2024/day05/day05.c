#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 5                                  //
// Link: https://adventofcode.com/2024/day/5                  //
////////////////////////////////////////////////////////////////

#define filename "day05input.txt"

// Parses the file and returns the number of rules
int parseRules(FILE * fp, int ** *rules) {
    int n = 0;
    char line[256];
    int a, b;
    int *tmp = NULL;

    // Read all the rules till the blank line
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d|%d\n", &a, &b) != 2) break;

        // Reallocate the array and add the new value
        *rules = (int **) realloc(*rules, (n + 1) * sizeof(int));
        tmp = (int *) calloc(2, sizeof(int));
        
        tmp[0] = a; tmp[1] = b;

        (*rules)[n++] = tmp;
    }

    return n;
}

int parseLine(FILE *fp, int **arr) {
    if (arr == NULL) return 0;

    // Initialize the array to a single element
    *arr = (int *) calloc(1, sizeof(int));

    char ch;
    int n = 0, i;
    char tmp[10];

    // Read till the end of the line or the end of the file
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (!isdigit(ch)) continue;

        i = 0;

        // Read the number digit by digit as a string
        do {
            if (i < sizeof(tmp) - 1) tmp[i++] = ch;
            ch = fgetc(fp);
        } while (isdigit(ch));
        tmp[i] = '\0';

        // Reallocate the array with one more element
        int *newArr = (int *) realloc(*arr, (n + 1) * sizeof(int));
        if (newArr == NULL) {
            // If realloc fails, free the array and exit
            perror("Memory allocation failed");
            free(*arr);
            *arr = NULL;
            exit(EXIT_FAILURE);
        }

        *arr = newArr;

        // Convert the string to an integer and add it to the array
        (*arr)[n++] = atoi(tmp);

        if (ch == EOF || ch == '\n') break;
    }

    return n;
}

// Checks if the array matches the rule a comes before b
int matches(int a, int b, int * arr, int n) {
    int posA = -1, posB = -1;

    // Find the positions of a and b
    for (int i = 0; i < n; i++) {
        if (arr[i] == a) posA = i;
        if (arr[i] == b) posB = i;
    }

    // If either a or b is not found the rule is enforced
    if (posA == -1 || posB == -1) return 1;
    if (posA < posB) return 1;

    return 0;
}

// Checks if the array matches all the rules
int enforce(int ** rules, int * arr, int nRules, int n) {
    for (int i = 0; i < nRules; i++) {
        // If any rule is not enforced, return 0
        if (matches(rules[i][0], rules[i][1], arr, n)) continue;

        return 0;
    }
    return 1;
}

// Returns the median of the array
int median(int * arr, int n) {
    return arr[n / 2];
}

// Returns 1 if a comes before b using the rules
int comesBefore(int a, int b, int ** rules, int nRules) {
    // Check if any rule where a comes before b exists
    for (int i = 0; i < nRules; i++) {
        if (rules[i][0] == a && rules[i][1] == b) return 1;
        if (rules[i][0] == b && rules[i][1] == a) return 0;
    }

    // If no rule is found, the smaller one comes first
    return a < b;
}

// Swaps two elements of the array
void swap(int *array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

// Sorts the array using selection sort
void sort(int *array, int n, int **rules, int nRules) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        // Find the index of the minimum element
        for (int j = i + 1; j < n; j++) {
            if (comesBefore(array[j], array[minIndex], rules, nRules)) minIndex = j;
        }
        
        // Swap the minimum element with the current element
        if (minIndex != i) swap(array, i, minIndex);
    }
}


int main(void) {
    // Open the file to read from
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error while opening file\n");
        return 1;
    }

    // Parse the rules from the file
    int ** rules = NULL;
    int nRules = parseRules(fp, &rules);

    // Initialize the counters of the medians for part 1 and part 2
    int count = 0,  count2 = 0;

    // Initialize the array to store the line
    int * arr = NULL;
    int n;

    // While there are lines in the file
    while ((n=parseLine(fp, &arr)) > 0) {
        // If the line matches all the rules, it's a valid update
        if (enforce(rules, arr, nRules, n)) {
            // Add the median of the valid update to the count
            count+= median(arr, n);
        } else {
            // Sort the invalid update using the rules
            sort(arr, n, rules, nRules);

            // Add the median of the sorted invalid update to the count
            count2+= median(arr, n);
        }

        // Free the array to avoid memory leaks
        free(arr);
    }

    fclose(fp);

    // Part 1: Find the sum of the medians of the valid updates
    printf("Part 1: \n\tSum of medians of valid updates: %d\n", count);

    // Part 2: Find the sum of the medians of the invalid updates after sorting them
    printf("Part 2: \n\tSum of medians of sorted invalid updates: %d\n", count2);

    free(rules);

    return 0;
}

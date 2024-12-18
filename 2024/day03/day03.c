#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 3                                  //
// Link: https://adventofcode.com/2024/day/3                  //
////////////////////////////////////////////////////////////////

#define filename "day03input.txt"

// Matches two integers in the form n1,n2)
int matchInt(FILE *fp) {
    int n1, n2;
    char ch;

    // Read two integers 
    if (fscanf(fp, "%d,%d", &n1, &n2) != 2) return 0;
    // Read the closing parenthesis, if not found return 0
    if (fgetc(fp) != ')') return 0;

    // Print the result of the multiplication
    return n1 * n2;
}

int readInstructions(void) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        // If the file can't be opened, print an error message
        printf("Error while opening file\n");
        return 1;
    }
    
    char ch;

    // Match the string "mul(" that is part of the instruction mul(x,y)
    char match[] = "mul(";

    int i = 0;
    int total = 0;

    // Read till the end of the of the file
    while ((ch = fgetc(fp)) != EOF) {

        // If the current character is the same as the current character of the match string, increment the index
        if (!(ch == match[i++])) i = 0;
        
        // If string has been matched, read two integers and add their product to the total
        if (i == 4) {
            total += matchInt(fp);
            i = 0;
        }
    }

    fclose(fp);

    return total;

}

int readInstructions2(void) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        // If the file can't be opened, print an error message
        printf("Error while opening file\n");
        return 1;
    }
    

    char ch;

    // Match the string "mul(" that is part of the instruction mul(x,y)
    char matchA[] = "mul(";
    int a = 0;

    // Match the string equivalent to the instruction do()
    char matchB[] = "do()";
    int b = 0;

    // Match the string equivalent to the instruction don't()
    char matchC[] = "don't()";
    int c = 0;

    int total = 0;
    int canDo = 1;	

    // Read till the end of the of the file
    while ((ch = fgetc(fp)) != EOF) {

        // If the current character is the same as the current character of the match string, increment the index
        if (!(ch == matchA[a++])) a = 0;
        if (!(ch == matchB[b++])) b = 0;
        if (!(ch == matchC[c++])) c = 0;

        // If string has been matched and the mul function is active, read two integers and add their product to the total
        if (a == 4 && canDo) {	
            total += matchInt(fp);
            a = 0;
        }

        // If string has been matched activate or deactivate the mul function
        if (b == 4) {canDo = 1; b = 0;}
        if (c == 7) {canDo = 0; c = 0;}
            
    }

    fclose(fp);

    return total;

}

int main(void) {
    // Part 1: Sum and execute all multiply instructions
    printf("Part 1: \n\tTotal: %d\n", readInstructions());

    // Part 2: Sum and execute all multiply instructions if do() is active
    printf("Part 2: \n\tTotal: %d", readInstructions2());

    return 0;
}

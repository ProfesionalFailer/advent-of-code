#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////
// Advent of Code 2024 Day 11                                 //
// Link: https://adventofcode.com/2024/day/11                 //
////////////////////////////////////////////////////////////////

#define filename "day11input.txt"

// Define a struct to represent a stone with a value and quantity
typedef struct stone {
    uint64_t val; 
    uint64_t quantity;
    struct stone *next; // Pointer to the next stone
} stone;

// Function to create a new stone node
stone *makeNode(uint64_t val, uint64_t quantity) {
    // Allocate memory for a new stone
    stone *node = malloc(sizeof(stone));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed in makeNode.\n");
        exit(EXIT_FAILURE); // Exit the program if memory allocation fails
    }

    // Initialize the stone with the provided value and quantity
    node->val = val;
    node->quantity = quantity;
    node->next = NULL;

    // Return the newly created stone node
    return node;
}

// Insert a new stone at the head of the list
void insOnHead(stone **head, stone *new) {
    new->next = *head;
    *head = new;
}

// Insert a new stone after a given stone node
void insAfter(stone *node, stone *new) {
    new->next = node->next;
    node->next = new;
}

// Insert a stone in a sorted order based on its value
void insInOrder(stone **head, uint64_t val, uint64_t quantity) {
    // If the list is empty, insert at the head
    if (*head == NULL) {
        insOnHead(head, makeNode(val, quantity));
        return;
    }

    // If the list has only one stone and it's greater than the new one
    // then insert it as the head to maintain the sorted order
    if ((*head)->next == NULL && (*head)->val > val) {
        insOnHead(head, makeNode(val, quantity));
        return;
    }

    // Traverse the list to find the last stone with a value less or equal than the new one
    stone *node = *head;
    while (node->next != NULL && node->next->val < val) {
        node = node->next;
    }

    // If a stone with the same value exists, update the quantity
    if (node->next != NULL && node->next->val == val) {
        node->next->quantity += quantity;
        return;
    }

    // Insert the new stone at the correct position
    insAfter(node, makeNode(val, quantity));
}

// Count the total quantity of stones in the list
uint64_t countStones(stone *head) {
    uint64_t count = 0;
    // For each stone in the list
    while (head != NULL) {
        count += head->quantity; // Add the quantity of current stone
        head = head->next;
    }
    return count; 
}

// Free the memory allocated for the list of stones
void freeStones(stone *head) {
    stone *temp;
    while (head != NULL) {
        temp = head;     // Store current head
        head = head->next; // Move to next stone
        free(temp);      // Free the current stone
    }
}

// Parse stones from a file and insert them into a linked list
stone *parseFile() {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    stone *head = NULL; // Initialize the head of the list

    uint64_t val;
    // Read values from the file and insert into the list
    while (fscanf(fp, "%llu", &val) == 1) {
        insInOrder(&head, val, 1);
    };

    fclose(fp); // Close the file

    return head; // Return the head of the list
}

// Count the number of digits in a number
uint8_t countDigits(uint64_t num) {
    uint8_t count = 0;

    // Keep dividing the number by 10 until it becomes 0, the number of digits is 
    // equal to the total number of divisions
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count; // Return the total number of digits
}

// Split a number into two parts
void splitNum(uint64_t num, uint64_t *left, uint64_t *right) {
    uint8_t digits = countDigits(num) / 2; // Calculate the number of digits and divide by 2

    uint64_t div = 1;

    // The divisor is the popwer of 10 that splits the number in half
    for (int i = 0; i < digits; i++) div *= 10;

    *left = num / div;  // Get the left part of the number
    *right = num % div; // Get the right part of the number
}

// Process a stone and add stones with the appropriate values
// to the new list
void blinkStone(stone *st, stone **new) {
    // If value is 0, add stones of value 1 and the same quantity
    if (st->val == 0) {
        insInOrder(new, 1, st->quantity);
        return;
    }
    // If the number of digits is odd, add stone with value multiplied by 2024 and the same quantity
    else if (countDigits(st->val) % 2 != 0) {
        insInOrder(new, st->val * 2024, st->quantity);
        return;
    }

    uint64_t left, right;
    splitNum(st->val, &left, &right); // Split the stone value

    // Insert the left and right parts into the new list
    insInOrder(new, left, st->quantity);
    insInOrder(new, right, st->quantity);
}

// Perform a series of blinks on the stone list
uint64_t blink(stone **head, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        stone *curr = *head; // Start with the head of the list

        stone *new = NULL; // Initialize a new list
        
        // Process each stone in the current list
        while (curr != NULL) {
            blinkStone(curr, &new);
            curr = curr->next;
        }

        freeStones(*head); // Free the old list
        *head = new;       // Update the head to the new list
        // This is done to avoid overlapping when processing the blink
    }

    return countStones(*head); // Return the count of stones after blinks
}

// Main function to drive the program
int main(void) {
    // In this scenario the linked list is used like an hashmap
    stone *head = parseFile(); // Parse the stones from the file

    // Part 1: Perform 25 blinks and print the number of stones
    printf("Part 1: \n\tNumber of stones after 25 blinks: %llu\n", blink(&head, 25));
    // Part 2: Perform additional 50 blinks and print the number of stones
    printf("Part 2: \n\tNumber of stones after 75 blinks: %llu\n", blink(&head, 50));

    freeStones(head); // Free the memory allocated for the stones

    return 0; // Return success
}

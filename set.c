//
// Created by f0rio2 on 1/10/24.
//
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include "cSha.h"

#define SIZE 10

bool exists(char *arr[], char *str, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i], str) == 0) {
            return true;
        }
    }
    return false;
}


void set(char *arr[], int size, int* uniqueCount) {
    char **result = malloc(size * sizeof(char *));
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (!exists(result, arr[i], count)) {
            result[count] = arr[i];
            count++;
        }
    }
    *uniqueCount = count;

    // Copy the unique elements back to arr[]
    for (int i = 0; i < count; i++) {
        arr[i] = result[i];
    }

    // Free the memory allocated for result
    free(result);
}
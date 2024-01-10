#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cSha.h"


int count_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(file);
    return count;
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void sort_file(const char *filename) {
    int lines_count = count_lines(filename);
    if (lines_count < 0) {
        return;
    }

    char **lines = malloc(lines_count * sizeof(char *));
    if (lines == NULL) {
        perror("Failed to allocate memory");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        free(lines);
        return;
    }

    size_t line_length;
    for (int i = 0; i < lines_count; i++) {
        line_length = 0;
        getline(&lines[i], &line_length, file);
    }

    fclose(file);

    qsort(lines, lines_count, sizeof(char *), compare);

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        for (int i = 0; i < lines_count; i++) {
            free(lines[i]);
        }
        free(lines);
        return;
    }

    for (int i = 0; i < lines_count; i++) {
        fputs(lines[i], file);
        free(lines[i]);
    }

    free(lines);
    fclose(file);
}
// Sample SW for PIPE TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LENGTH 100

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    char *lines[MAX_LINES];
    char buffer[MAX_LENGTH];
    int count = 0;

    // Read input lines
    while (fgets(buffer, MAX_LENGTH, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        lines[count] = malloc(strlen(buffer) + 1);
        if (lines[count] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        strcpy(lines[count], buffer);
        count++;
        if (count >= MAX_LINES) {
            break;
        }
    }

    // Sort lines
    qsort(lines, count, sizeof(char *), compare);

    // Print sorted lines
    for (int i = 0; i < count; i++) {
        printf("%s\n", lines[i]);
        free(lines[i]); // Free allocated memory
    }

    return 0;
}
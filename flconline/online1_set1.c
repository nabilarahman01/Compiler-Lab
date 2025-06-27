#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1000

int countFunctions(const char *filename);

int main() {
    const char *filename = "online1_set1.txt";  // Input file containing the C source code
    int functionCount = countFunctions(filename);

    printf("No. of functions: %d\n", functionCount);
    return 0;
}

int countFunctions(const char *filename) {
    FILE *file;
    char line[MAX_LEN];
    int functionCount = 0;

    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    while (fgets(line, MAX_LEN, file) != NULL) {
        char *ptr = line;

        // Skip leading spaces or tabs
        while (*ptr && isspace(*ptr)) ptr++;

        // Check if the line contains an opening parenthesis followed by a closing parenthesis before a semicolon
        char *openParen = strchr(ptr, '(');
        char *closeParen = strchr(ptr, ')');

        // If there's a potential function definition or call
        if (openParen && closeParen && (closeParen > openParen)) {
            char *semicolon = strchr(closeParen, ';');
            char *brace = strchr(closeParen, '{');

            // If there's no semicolon after the closing parenthesis, it's likely a function definition
            if (!semicolon || (brace && brace < semicolon)) {
                functionCount++;
            }
        }
    }

    fclose(file);
    return functionCount;
}


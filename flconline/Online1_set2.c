#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function to check if a lexeme is a floating point number matching d*.dd
int isFloatingPoint(const char *lexeme) {
    int i = 0;
    int hasDigitBefore = 0;
    int decimalCount = 0;

    // Check digits before the decimal point (at least one digit required)
    while (isdigit(lexeme[i])) {
        hasDigitBefore = 1;
        i++;
    }

    // Check for the decimal point
    if (lexeme[i] == '.') {
        decimalCount++;
        i++;
    }

    // Check for exactly two digits after the decimal point
    if (decimalCount == 1 && isdigit(lexeme[i]) && isdigit(lexeme[i + 1]) && !isdigit(lexeme[i + 2])) {
        i += 2;

        // Ensure the lexeme ends here (no extra characters)
        if (lexeme[i] == '\0' && hasDigitBefore) {
            return 1;
        }
    }

    // If not a valid floating point number
    return 0;
}

int main(void) {
    FILE *file;
    char word[50];
    int count = 0;

    // Open the input file
    file = fopen("inputdfa.txt", "r");
    if (!file) {
        printf("File can't be opened!\n");
        return 1;
    }

    // Read words (lexemes) from the file
    while (fscanf(file, "%s", word) != EOF) {
        if (isFloatingPoint(word)) {
            count++;
        }
    }

    // Print the result
    printf("Number of floating point numbers: %d\n", count);

    // Close the file
    fclose(file);

    return 0;
}

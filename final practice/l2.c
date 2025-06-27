#include <stdio.h>
#include <string.h>

// Function to check if the entered string is a keyword
int is_keyword(const char *lexeme) {
    // List of C keywords
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
        "void", "volatile", "while"
    };

    // Total number of keywords
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    // Compare the entered string with the list of keywords
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;  // It is a keyword
        }
    }
    return 0;  // Not a keyword
}

int main() {
    char lexeme[50];

    // Input: Get a string from the user
    printf("Enter a string: ");
    scanf("%s", lexeme);

    // Check if the entered string is a keyword
    if (is_keyword(lexeme)) {
        printf("%s is a keyword.\n", lexeme);
    } else {
        printf("%s is not a keyword.\n", lexeme);
    }

    return 0;
}

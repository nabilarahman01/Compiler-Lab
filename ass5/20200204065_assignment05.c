#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Global variables to keep track of the input string and current position
const char *expression;
int index = 0;

// Function prototypes
int evaluateExpression();
int evaluateTerm();
int evaluateFactor();
int evaluateIdentifier();
int evaluateNumber();

// Function to match the current character with the expected one
int matchCharacter(char expected) {
    if (expression[index] == expected) {
        index++;
        return 1;
    }
    return 0;
}

// Parse expression: <Expression> => <Term> + <Term> | <Term> - <Term> | <Term>
int evaluateExpression() {
    int initialIndex = index;
    if (evaluateTerm()) {
        if (matchCharacter('+') || matchCharacter('-')) {
            if (evaluateTerm()) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    index = initialIndex;
    return 0;
}

// Parse term: <Term> => <Factor> * <Factor> | <Factor> / <Factor> | <Factor>
int evaluateTerm() {
    int initialIndex = index;
    if (evaluateFactor()) {
        if (matchCharacter('*') || matchCharacter('/')) {
            if (evaluateFactor()) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    index = initialIndex;
    return 0;
}

// Parse factor: <Factor> => ( <Expression> ) | ID | NUM
int evaluateFactor() {
    int initialIndex = index;
    if (matchCharacter('(')) {
        if (evaluateExpression() && matchCharacter(')')) {
            return 1;
        }
    }
    index = initialIndex;
    if (evaluateIdentifier() || evaluateNumber()) {
        return 1;
    }
    index = initialIndex;
    return 0;
}

// Parse identifier: ID => a|b|c|d|e
int evaluateIdentifier() {
    if (expression[index] >= 'a' && expression[index] <= 'e') {
        index++;
        return 1;
    }
    return 0;
}

// Parse number: NUM => 0|1|2|...|9
int evaluateNumber() {
    if (isdigit(expression[index])) {
        index++;
        return 1;
    }
    return 0;
}

// Main function to test the parser
int main() {
    char input[1000];
    printf("Enter an arithmetic expression: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    expression = input;
    index = 0;

    if (evaluateExpression() && expression[index] == '\0') {
        printf("\nThe expression is VALID.\n");
    } else {
        printf("\nThe expression is INVALID.\n");
    }

    return 0;
}

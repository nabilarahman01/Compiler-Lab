#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Global variables to keep track of the input string and current position
const char* str;
int i = 0;
int f = 1;  // flag to indicate success or failure

// Function prototypes
void Exp();
void Term();
void Factor();

// Function to check if the current character matches the expected one
void match(char expected) {
    if (str[i] == expected) {
        i++;
    } else {
        f = 0;  // Error
    }
}

// <Exp>-><Term> + <Term> | <Term> - <Term> | <Term>
void Exp() {
    Term();
    if (f && (str[i] == '+' || str[i] == '-')) {
        char op = str[i];
        match(op);
        Term();
    }
}

// <Term>-><Factor> * <Factor> | <Factor> / <Factor> | <Factor>
void Term() {
    Factor();
    if (f && (str[i] == '*' || str[i] == '/')) {
        char op = str[i];
        match(op);
        Factor();
    }
}

// <Factor>-> ( <Exp> ) | ID | NUM
void Factor() {
    if (str[i] == '(') {
        match('(');
        Exp();
        match(')');
    } else if (str[i] >= 'a' && str[i] <= 'e') {  // ID (only a, b, c, d, e are valid)
        match(str[i]);
    } else if (isdigit(str[i])) {  // NUM
        while (isdigit(str[i])) {
            match(str[i]);
        }
    } else {
        f = 0;  // Error
    }
}


int main() {
    char input[1000];
    printf("Enter an arithmetic expression: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    str = input;
    i = 0;
    f = 1;
    Exp();
    if (f && str[i] == '\0') {
        printf("\nThe string is a valid arithmetic expression.\n");
    } else {
        printf("\nThe string is not a valid arithmetic expression.\n");
    }

    return 0;
}

#include <stdio.h>
#include <string.h>

// Global variables to keep track of the input string and current position
const char* str;
int i = 0;
int f = 1;  // flag to indicate success or failure

// Function prototypes
void S();
void A();
void B();

// Function to check if the current character matches the expected one
void match(char expected) {
    if (str[i] == expected) {
        i++;
    } else {
        f = 0;  // Error
    }
}

// S -> vAz | xB
void S() {
    if (str[i] == 'v') {
        match('v');
        if (f) A();
        if (f) match('z');
    } else if (str[i] == 'x') {
        match('x');
        if (f) B();
    } else {
        f = 0;  // Error
    }
}

// A -> xy | yB
void A() {
    if (str[i] == 'x' && str[i + 1] == 'y') {
        match('x');
        if (f) match('y');
    } else if (str[i] == 'y') {
        match('y');
        if (f) B();
    } else {
        f = 0;  // Error
    }
}

// B -> z
void B() {
    match('z');
}

// Main function to test the parser
int main() {
    char input[1000];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    str = input;
    i = 0;
    f = 1;
    S();
    if (f && str[i] == '\0') {
        printf("String is valid.\n");
    } else {
        printf("String is invalid.\n");
    }

    return 0;
}

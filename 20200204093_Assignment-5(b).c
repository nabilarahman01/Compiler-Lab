#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


const char *input;
int pos = 0;

int parseExp();
int parseTerm();
int parseFactor();
int parseID();
int parseNUM();

int match(char expected) {
    if (input[pos] == expected) {
        pos++;
        return 1;
    }
    return 0;
}

int parseExp() {
    int initialPos = pos;
    if (parseTerm()) {
        if (match('+') || match('-')) {
            if (parseTerm()) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    pos = initialPos;
    return 0;
}

int parseTerm() {
    int initialPos = pos;
    if (parseFactor()) {
        if (match('*') || match('/')) {
            if (parseFactor()) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    pos = initialPos;
    return 0;
}

int parseFactor() {
    int initialPos = pos;
    if (match('(')) {
        if (parseExp() && match(')')) {
            return 1;
        }
    }
    pos = initialPos;
    if (parseID() || parseNUM()) {
        return 1;
    }
    pos = initialPos;
    return 0;
}

int parseID() {
    if (input[pos] >= 'a' && input[pos] <= 'e') {
        pos++;
        return 1;
    }
    return 0;
}

int parseNUM() {
    if (isdigit(input[pos])) {
        pos++;
        return 1;
    }
    return 0;
}


int main() {

    char expression[1000];
    printf("\n\nEnter an arithmetic expression: ");
    fgets(expression, sizeof(expression), stdin);

    // Remove newline character if present
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }

    input = expression;
    pos = 0;

    if (parseExp() && input[pos] == '\0') {
        printf("\nThe expression is accepted\n");
    } else {
        printf("\nThe expression is not accepted\n");
    }


    return 0;
}

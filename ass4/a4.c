#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LEN 100
#define MAX_SYMBOLS 100

typedef struct {
    int id;
    char name[MAX_TOKEN_LEN];
    char idType[MAX_TOKEN_LEN];
    char dataType[MAX_TOKEN_LEN];
    char scope[MAX_TOKEN_LEN];
    char value[MAX_TOKEN_LEN];
    int line;
} Symbol;

typedef struct {
    char token[MAX_TOKEN_LEN];
    char type[MAX_TOKEN_LEN];
    int line;
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void insert(char *name, char *idType, char *dataType, char *scope, char *value, int line) {
    symbolTable[symbolCount].id = symbolCount + 1;
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].idType, idType);
    strcpy(symbolTable[symbolCount].dataType, dataType);
    strcpy(symbolTable[symbolCount].scope, scope);
    strcpy(symbolTable[symbolCount].value, value);
    symbolTable[symbolCount].line = line;
    symbolCount++;
}

Symbol* search(char *name, char *scope) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && strcmp(symbolTable[i].scope, scope) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

void processInput(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    char line[256];
    int lineNumber = 1;
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " []\n");
        while (token != NULL) {
            if (strcmp(token, "kw") == 0 || strcmp(token, "id") == 0 || strcmp(token, "op") == 0 ||
                strcmp(token, "sep") == 0 || strcmp(token, "par") == 0 || strcmp(token, "brc") == 0 ||
                strcmp(token, "num") == 0) {
                Token newToken;
                strcpy(newToken.type, token);
                token = strtok(NULL, " []\n");
                if (token != NULL) {
                    strcpy(newToken.token, token);
                    newToken.line = lineNumber;
                    tokens[tokenCount++] = newToken;
                }
            }
            token = strtok(NULL, " []\n");
        }
        lineNumber++;
    }

    fclose(file);
}

void detectErrors() {
    int braceCount = 0;
    int parenCount = 0;
    int inElseBlock = 0;
    char scope[MAX_TOKEN_LEN] = "global";

    for (int i = 0; i < tokenCount; i++) {
        if (strcmp(tokens[i].type, "kw") == 0) {
            if (strcmp(tokens[i].token, "float") == 0 || strcmp(tokens[i].token, "double") == 0 || strcmp(tokens[i].token, "int") == 0) {
                if (i + 1 < tokenCount && strcmp(tokens[i+1].type, "id") == 0) {
                    char name[MAX_TOKEN_LEN];
                    strcpy(name, tokens[i+1].token);
                    if (search(name, scope)) {
                        printf("Error: Duplicate declaration of identifier '%s' at line %d\n", name, tokens[i+1].line);
                    }
                    if (i + 2 < tokenCount && strcmp(tokens[i+2].token, "(") == 0) {
                        insert(name, "func", tokens[i].token, scope, "", tokens[i+1].line);
                        strcpy(scope, name);
                    } else {
                        insert(name, "var", tokens[i].token, scope, "", tokens[i+1].line);
                    }
                }
            }
        } else if (strcmp(tokens[i].type, "id") == 0 && i + 1 < tokenCount && strcmp(tokens[i+1].token, "=") == 0) {
            Symbol *sym = search(tokens[i].token, scope);
            if (sym) {
                strcpy(sym->value, tokens[i+2].token);
            }
        } else if (strcmp(tokens[i].type, "brc") == 0) {
            if (strcmp(tokens[i].token, "{") == 0) {
                braceCount++;
            } else if (strcmp(tokens[i].token, "}") == 0) {
                braceCount--;
                if (braceCount < 0) {
                    printf("Error: Unmatched closing brace at line %d\n", tokens[i].line);
                    braceCount = 0;
                }
                if (inElseBlock) {
                    inElseBlock = 0;
                }
            }
        } else if (strcmp(tokens[i].type, "par") == 0) {
            if (strcmp(tokens[i].token, "(") == 0) {
                parenCount++;
            } else if (strcmp(tokens[i].token, ")") == 0) {
                parenCount--;
                if (parenCount < 0) {
                    printf("Error: Unmatched closing parenthesis at line %d\n", tokens[i].line);
                    parenCount = 0;
                }
            }
        } else if (strcmp(tokens[i].type, "sep") == 0 && strcmp(tokens[i].token, ";") == 0) {
            if (i > 0 && strcmp(tokens[i-1].type, "sep") == 0 && strcmp(tokens[i-1].token, ";") == 0) {
                printf("Error: Duplicate semicolon at line %d\n", tokens[i].line);
            }
        } else if (strcmp(tokens[i].type, "kw") == 0 && strcmp(tokens[i].token, "else") == 0) {
            if (!inElseBlock) {
                inElseBlock = 1;
            } else {
                printf("Error: Unmatched 'else' at line %d\n", tokens[i].line);
            }
        }
    }

    if (braceCount != 0) {
        printf("Error: Unbalanced braces\n");
    }
    if (parenCount != 0) {
        printf("Error: Unbalanced parentheses\n");
    }
}

int main() {
    processInput("in3.txt");
    detectErrors();
    return 0;
}

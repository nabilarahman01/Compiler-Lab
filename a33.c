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
} Symbol;

typedef struct {
    char token[MAX_TOKEN_LEN];
    char type[MAX_TOKEN_LEN];
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

void insert(char *name, char *idType, char *dataType, char *scope, char *value) {
    symbolTable[symbolCount].id = symbolCount + 1;
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].idType, idType);
    strcpy(symbolTable[symbolCount].dataType, dataType);
    strcpy(symbolTable[symbolCount].scope, scope);
    strcpy(symbolTable[symbolCount].value, value);
    symbolCount++;
}

void update(int id, char *name, char *idType, char *dataType, char *scope, char *value) {
    if (id > 0 && id <= symbolCount) {
        if (name) strcpy(symbolTable[id-1].name, name);
        if (idType) strcpy(symbolTable[id-1].idType, idType);
        if (dataType) strcpy(symbolTable[id-1].dataType, dataType);
        if (scope) strcpy(symbolTable[id-1].scope, scope);
        if (value) strcpy(symbolTable[id-1].value, value);
    }
}

void delete(int id) {
    if (id > 0 && id <= symbolCount) {
        for (int i = id - 1; i < symbolCount - 1; i++) {
            symbolTable[i] = symbolTable[i + 1];
        }
        symbolCount--;
    }
}

Symbol* search(char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

void display() {
    printf("Sl. No.\tName\tId Type\tData Type\tScope\tValue\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%d\t%s\t%s\t%s\t%s\t%s\n",
               symbolTable[i].id, symbolTable[i].name, symbolTable[i].idType,
               symbolTable[i].dataType, symbolTable[i].scope, symbolTable[i].value);
    }
}

void processInput(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    char line[256];
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
                    tokens[tokenCount++] = newToken;
                }
            }
            token = strtok(NULL, " []\n");
        }
    }

    fclose(file);
}

void generateOutput1(char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < tokenCount; i++) {
        if (strcmp(tokens[i].type, "id") == 0) {
            fprintf(file, "[id %s] ", tokens[i].token);
        } else {
            fprintf(file, "[%s] ", tokens[i].token);
        }
    }

    fclose(file);
}

void generateSymbolTable() {
    char scope[MAX_TOKEN_LEN] = "global";
    for (int i = 0; i < tokenCount; i++) {
        if (strcmp(tokens[i].type, "kw") == 0) {
            if (strcmp(tokens[i].token, "float") == 0 || strcmp(tokens[i].token, "double") == 0 || strcmp(tokens[i].token, "int") == 0) {
                char dataType[MAX_TOKEN_LEN];
                strcpy(dataType, tokens[i].token);
                if (strcmp(tokens[i+1].type, "id") == 0) {
                    char name[MAX_TOKEN_LEN];
                    strcpy(name, tokens[i+1].token);
                    if (strcmp(tokens[i+2].token, "(") == 0) {
                        insert(name, "func", dataType, scope, "");
                        strcpy(scope, name);
                    } else {
                        char value[MAX_TOKEN_LEN] = "";
                        if (strcmp(tokens[i+3].token, "=") == 0) {
                            strcpy(value, tokens[i+4].token);
                        }
                        insert(name, "var", dataType, scope, value);
                    }
                }
            } else if (strcmp(tokens[i].token, "return") == 0) {
                strcpy(scope, "global");
            }
        } else if (strcmp(tokens[i].type, "brc") == 0 && strcmp(tokens[i].token, "}") == 0) {
            strcpy(scope, "global");
        }
    }
}

void generateOutput2(char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    fprintf(file, "Sl. No.\tName\tId Type\tData Type\tScope\tValue\n");
    for (int i = 0; i < symbolCount; i++) {
        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%s\n",
                symbolTable[i].id, symbolTable[i].name, symbolTable[i].idType,
                symbolTable[i].dataType, symbolTable[i].scope, symbolTable[i].value);
    }

    fclose(file);
}

void generateOutput3(char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < tokenCount; i++) {
        if (strcmp(tokens[i].type, "id") == 0) {
            Symbol *sym = search(tokens[i].token);
            if (sym) {
                fprintf(file, "[id %d] ", sym->id);
            }
        } else {
            fprintf(file, "[%s] ", tokens[i].token);
        }
    }

    fclose(file);
}

int main() {
    processInput("in3.txt");
    generateOutput1("out1.txt");
    generateSymbolTable();
    generateOutput2("out2.txt");
    generateOutput3("out3.txt");

    return 0;
}

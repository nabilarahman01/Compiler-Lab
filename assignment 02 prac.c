#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEXEME_LEN 100
#define MAX_INPUT_LEN 1000

// Check if a lexeme is a keyword
int is_keyword(const char *lexeme) {
    if (strcmp(lexeme, "char") == 0 || strcmp(lexeme, "int") == 0 ||
        strcmp(lexeme, "float") == 0 || strcmp(lexeme, "if") == 0 ||
        strcmp(lexeme, "else") == 0) {
        return 1;
    }
    return 0;
}

// DFA to recognize numeric constants
int num_rec(const char *lex) {
    int i = 0, l, s;
    if (isdigit(lex[i])) {
        s = 1;
        i++;
    } else if (lex[i] == '.') {
        s = 2;
        i++;
    } else {
        s = 0;
    }

    l = strlen(lex);

    if (s == 1) {
        for (; i < l; i++) {
            if (isdigit(lex[i])) {
                s = 1;
            } else if (lex[i] == '.') {
                s = 2;
                i++;
                break;
            } else {
                s = 0;
                break;
            }
        }
    }

    if (s == 2) {
        if (isdigit(lex[i])) {
            s = 3;
            i++;
        } else {
            s = 0;
        }
    }

    if (s == 3) {
        for (; i < l; i++) {
            if (isdigit(lex[i])) {
                s = 3;
            } else {
                s = 0;
                break;
            }
        }
    }

    if (s == 3) {
        s = 1;
    }

    return s;
}

// Check if a lexeme is an identifier
int is_identifier(const char *lexeme) {
    if (!isalpha(lexeme[0]) && lexeme[0] != '_') {
        return 0;
    }
    for (int i = 1; lexeme[i] != '\0'; i++) {
        if (!isalnum(lexeme[i]) && lexeme[i] != '_') {
            return 0;
        }
    }
    return 1;
}

// Check if a lexeme is an operator
int is_operator(const char *lexeme) {
    if (strcmp(lexeme, "+") == 0 || strcmp(lexeme, "-") == 0 ||
        strcmp(lexeme, "*") == 0 || strcmp(lexeme, "/") == 0 ||
        strcmp(lexeme, "=") == 0 || strcmp(lexeme, "<=") == 0) {
        return 1;
    }
    return 0;
}

// Check if a lexeme is a separator
int is_separator(const char *lexeme) {
    if (strcmp(lexeme, ";") == 0 || strcmp(lexeme, ",") == 0 ||
        strcmp(lexeme, "(") == 0 || strcmp(lexeme, ")") == 0 ||
        strcmp(lexeme, "'") == 0) {
        return 1;
    }
    return 0;
}

// Tokenize the input string
void tokenize(FILE *inputFile, FILE *outputFile) {
    char lexeme[MAX_LEXEME_LEN];
    int i = 0, j;
    char c;

    while ((c = fgetc(inputFile)) != EOF) {
        if (isspace(c)) {
            continue;
        }

        // Collect lexeme
        j = 0;
        if (isalpha(c) || c == '_') {
            while (isalnum(c) || c == '_') {
                lexeme[j++] = c;
                c = fgetc(inputFile);
            }
            ungetc(c, inputFile);
        } else if (isdigit(c) || c == '.') {
            while (isdigit(c) || c == '.') {
                lexeme[j++] = c;
                c = fgetc(inputFile);
            }
            ungetc(c, inputFile);
        } else if (c == '<' && (c = fgetc(inputFile)) == '=') {
            lexeme[j++] = '<';
            lexeme[j++] = '=';
        } else {
            lexeme[j++] = c;
        }
        lexeme[j] = '\0';

        // Classify lexeme
        if (is_keyword(lexeme)) {
            fprintf(outputFile, "[kw %s] ", lexeme);
            printf("[kw %s] ", lexeme);
        } else if (is_identifier(lexeme)) {
            fprintf(outputFile, "[id %s] ", lexeme);
            printf("[id %s] ", lexeme);
        } else if (num_rec(lexeme)) {
            fprintf(outputFile, "[num %s] ", lexeme);
            printf("[num %s] ", lexeme);
        } else if (is_operator(lexeme)) {
            fprintf(outputFile, "[op %s] ", lexeme);
            printf("[op %s] ", lexeme);
        } else if (is_separator(lexeme)) {
            fprintf(outputFile, "[sep %s] ", lexeme);
            printf("[sep %s] ", lexeme);
        } else {
            fprintf(outputFile, "[unkn %s] ", lexeme);
            fprintf(outputFile, "ERROR: Unknown lexeme %s\n", lexeme);
            printf("[unkn %s] ", lexeme);
            printf("ERROR: Unknown lexeme %s\n", lexeme);
        }
    }
    fprintf(outputFile, "\n");
    printf("\n");
}

int main() {
    FILE *p1, *p2, *p3, *p4;

    // Open input file for reading
    p1 = fopen("input 02.txt", "r");
    if (p1 == NULL) {
        printf("Error: Could not open input file\n");
        return 1;
    }

    // Open output file for writing
    p2 = fopen("output 02.txt", "w");
    if (p2 == NULL) {
        printf("Error: Could not open output file\n");
        fclose(p1);
        return 1;
    }

    // Tokenize the input and write to output file
    tokenize(p1, p2);

    // Close the files
    fclose(p1);
    fclose(p2);

    return 0;
}

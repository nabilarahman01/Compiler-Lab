#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_KEYWORDS 100

char *keywords[MAX_KEYWORDS];
int keyword_count = 0;

void load_keywords(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open keywords file\n");
        exit(1);
    }

    char line[50];
    while (fgets(line, sizeof(line), file) && keyword_count < MAX_KEYWORDS) {
        line[strcspn(line, "\n")] = '\0';
        keywords[keyword_count++] = strdup(line);
    }
    fclose(file);
}

int is_keyword(const char *lexeme) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


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


int is_operator(const char *lexeme) {
    if (strcmp(lexeme, "+") == 0 || strcmp(lexeme, "-") == 0 ||
        strcmp(lexeme, "*") == 0 || strcmp(lexeme, "/") == 0 ||
        strcmp(lexeme, "=") == 0 || strcmp(lexeme, "<=") == 0) {
        return 1;
    }
    return 0;
}


int is_parenthesis(const char *lexeme) {
    if (strcmp(lexeme, "(") == 0 || strcmp(lexeme, ")") == 0 ||
        strcmp(lexeme, "[") == 0 || strcmp(lexeme, "]") == 0 ||
        strcmp(lexeme, "{") == 0 || strcmp(lexeme, "}") == 0 ||
        strcmp(lexeme, "<") == 0 || strcmp(lexeme, ">") == 0) {
        return 1;
    }
    return 0;
}


int is_separator(const char *lexeme) {
    if (strcmp(lexeme, ";") == 0 || strcmp(lexeme, ",") == 0 ||
        strcmp(lexeme, "'") == 0 || strcmp(lexeme, "\"") == 0) {
        return 1;
    }
    return 0;
}


void tokenize(FILE *inputFile, FILE *outputFile) {
    char lexeme[100];
    int i = 0, j;
    char c;

    while ((c = fgetc(inputFile)) != EOF) {
        if (isspace(c)) {
            continue;
        }

        j = 0;
        if (c == '#') {

            lexeme[j++] = c;
            while ((c = fgetc(inputFile)) != ' ' && c != EOF) {
                lexeme[j++] = c;
            }
            lexeme[j] = '\0';
            fprintf(outputFile, "[pp %s] ", lexeme);
            printf("[pp %s] ", lexeme);
            continue;
        }


        if (isalpha(c) || c == '_') {
            while (isalnum(c) || c == '_') {
                lexeme[j++] = c;
                c = fgetc(inputFile);
            }
            lexeme[j] = '\0';
            ungetc(c, inputFile);
        } else if (isdigit(c) || c == '.') {
            while (isdigit(c) || c == '.') {
                lexeme[j++] = c;
                c = fgetc(inputFile);
            }
            lexeme[j] = '\0';
            ungetc(c, inputFile);
        } else {
            lexeme[j++] = c;
            lexeme[j] = '\0';

            char next = fgetc(inputFile);
            if (next != EOF) {
                lexeme[j++] = next;
                lexeme[j] = '\0';

                if (!is_operator(lexeme)) {
                    ungetc(next, inputFile);
                    lexeme[--j] = '\0';
                }
            }

            if (is_operator(lexeme)) {
                fprintf(outputFile, "[op %s] ", lexeme);
                printf("[op %s] ", lexeme);
            } else if (is_parenthesis(lexeme)) {
                fprintf(outputFile, "[par %s] ", lexeme);
                printf("[par %s] ", lexeme);
            } else if (is_separator(lexeme)) {
                fprintf(outputFile, "[sep %s] ", lexeme);
                printf("[sep %s] ", lexeme);
            } else {
                fprintf(outputFile, "[unkn %s] ", lexeme);
                fprintf(outputFile, "ERROR: Unknown lexeme %s\n", lexeme);
                printf("[unkn %s] ", lexeme);
                printf("ERROR: Unknown lexeme %s\n", lexeme);
            }
            continue;
        }


        if (is_keyword(lexeme)) {
            fprintf(outputFile, "[kw %s] ", lexeme);
            printf("[kw %s] ", lexeme);
        } else if (is_identifier(lexeme)) {
            fprintf(outputFile, "[id %s] ", lexeme);
            printf("[id %s] ", lexeme);
        } else if (num_rec(lexeme)) {
            fprintf(outputFile, "[num %s] ", lexeme);
            printf("[num %s] ", lexeme);
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
    FILE *p1, *p2;

    load_keywords("keywords.txt");

    p1 = fopen("output.txt", "r");
    if (p1 == NULL) {
        printf("Error: Could not open input file\n");
        return 1;
    }


    p2 = fopen("output 03.txt", "w");


    tokenize(p1, p2);


    fclose(p1);
    fclose(p2);

    return 0;
}


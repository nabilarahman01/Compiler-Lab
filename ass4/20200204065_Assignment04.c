#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_LENGTH 100
#define MAX_LINE_LENGTH 1000

typedef enum {
    IDENTIFIER,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    LITERAL,
    COMMENT,
    UNKNOWN
} TokenType;

typedef struct {
    char token[MAX_TOKEN_LENGTH];
    TokenType type;
    int line;
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;
char *keywords[] = {"int", "float", "if", "else", "while", "return", "void"};

void add_token(char *lexeme, TokenType type, int line) {
    strcpy(tokens[token_count].token, lexeme);
    tokens[token_count].type = type;
    tokens[token_count].line = line;
    token_count++;
}

int is_keyword(char *lexeme) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void tokenize(char *source_code) {
    char buffer[MAX_TOKEN_LENGTH];
    int line = 1;
    int pos = 0;

    while (*source_code) {
        if (isspace(*source_code)) {
            if (*source_code == '\n') {
                line++;
            }
            source_code++;
            continue;
        }

        if (isalpha(*source_code) || *source_code == '_') {
            pos = 0;
            while (isalnum(*source_code) || *source_code == '_') {
                buffer[pos++] = *source_code++;
            }
            buffer[pos] = '\0';
            if (is_keyword(buffer)) {
                add_token(buffer, KEYWORD, line);
            } else {
                add_token(buffer, IDENTIFIER, line);
            }
        } else if (isdigit(*source_code)) {
            pos = 0;
            while (isdigit(*source_code)) {
                buffer[pos++] = *source_code++;
            }
            buffer[pos] = '\0';
            add_token(buffer, LITERAL, line);
        } else if (*source_code == '/' && *(source_code + 1) == '/') {
            while (*source_code && *source_code != '\n') {
                source_code++;
            }
        } else if (*source_code == '/' && *(source_code + 1) == '*') {
            source_code += 2;
            while (*source_code && !(*source_code == '*' && *(source_code + 1) == '/')) {
                if (*source_code == '\n') {
                    line++;
                }
                source_code++;
            }
            if (*source_code) {
                source_code += 2;
            }
        } else {
            buffer[0] = *source_code;
            buffer[1] = '\0';
            if (buffer[0] == '{' || buffer[0] == '}' || buffer[0] == ';' || buffer[0] == '(' || buffer[0] == ')') {
                add_token(buffer, SEPARATOR, line);
            } else {
                add_token(buffer, UNKNOWN, line);
            }
            source_code++;
        }
    }
}

void syntax_analysis(FILE *output_file) {
    int brace_count = 0;
    int unmatched_else = 0;
    int unmatched_else_line = 0;

    for (int i = 0; i < token_count; i++) {
        // Check for duplicate tokens
        if (tokens[i].type == SEPARATOR && tokens[i].token[0] == ';' &&
            i > 0 && tokens[i - 1].type == SEPARATOR && tokens[i - 1].token[0] == ';') {
            fprintf(output_file, "Duplicate token ';' at line %d\n", tokens[i].line);
            printf("Duplicate token ';' at line %d\n", tokens[i].line);
        }

        // Check for misplaced '}'
        if (tokens[i].type == SEPARATOR && tokens[i].token[0] == '}') {
            brace_count--;
            if (brace_count < 0) {
                fprintf(output_file, "Misplaced '}' at line %d\n", tokens[i].line);
                printf("Misplaced '}' at line %d\n", tokens[i].line);
                brace_count = 0;
            }
        }

        // Track braces
        if (tokens[i].type == SEPARATOR && tokens[i].token[0] == '{') {
            brace_count++;
        }

        // Track 'else' statements
        if (tokens[i].type == KEYWORD && strcmp(tokens[i].token, "else") == 0) {
            unmatched_else++;
            unmatched_else_line = tokens[i].line;
        }

        // Match 'else' with 'if'
        if (tokens[i].type == KEYWORD && strcmp(tokens[i].token, "if") == 0) {
            unmatched_else = 0;
        }
    }

    // Report any unmatched braces
    if (brace_count > 0) {
        fprintf(output_file, "Unmatched '{' found\n");
        printf("Unmatched '{' found\n");
    }

    // Check for unmatched 'else'
    if (unmatched_else > 0) {
        fprintf(output_file, "Unmatched 'else' at line %d\n", unmatched_else_line);
        printf("Unmatched 'else' at line %d\n", unmatched_else_line);
    }
}

void generate_intermediate_file(const char *source_filename, const char *intermediate_filename) {
    FILE *source_file = fopen(source_filename, "r");
    FILE *intermediate_file = fopen(intermediate_filename, "w");
    if (source_file == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }
    if (intermediate_file == NULL) {
        perror("Error opening intermediate file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int current_line = 1;
    int token_index = 0;

    while (fgets(line, sizeof(line), source_file)) {
        fprintf(intermediate_file, "%d ", current_line);
        int printed_anything = 0;
        while (token_index < token_count && tokens[token_index].line == current_line) {
            if (tokens[token_index].type == KEYWORD) {
                fprintf(intermediate_file, "kw %s ", tokens[token_index].token);
            } else if (tokens[token_index].type == IDENTIFIER) {
                fprintf(intermediate_file, "id %s ", tokens[token_index].token);
            } else if (tokens[token_index].type == SEPARATOR || tokens[token_index].type == UNKNOWN) {
                fprintf(intermediate_file, "%s ", tokens[token_index].token);
            } else if (tokens[token_index].type == LITERAL) {
                fprintf(intermediate_file, "%s ", tokens[token_index].token);
            }
            token_index++;
            printed_anything = 1;
        }
        if (!printed_anything) {
            fprintf(intermediate_file, " ");
        }
        fprintf(intermediate_file, "\n");
        current_line++;
    }

    fclose(source_file);
    fclose(intermediate_file);
}

void read_source_code(const char *filename, char *source_code) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        strcat(source_code, line);
    }

    fclose(file);
}

int main() {
    const char *source_filename = "source.c";
    const char *intermediate_filename = "intermediate.txt";
    const char *output_filename = "output.txt";
    char source_code[10000] = "";

    read_source_code(source_filename, source_code);
    tokenize(source_code);
    generate_intermediate_file(source_filename, intermediate_filename);

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    syntax_analysis(output_file);
    fclose(output_file);

    return 0;
}

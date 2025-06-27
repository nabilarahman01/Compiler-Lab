#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int sl_no;
    char name[50];
    char id[10];
    char data_type[10];
    char scope[50];
    char value[50];
} Symbol;

typedef struct
{
    Symbol symbols[100];
    int count;
} SymbolTable;

void insertOrUpdate(SymbolTable *table, Symbol symbol);
void updateSymbol(SymbolTable *table, char *name, Symbol newSymbol);
void deleteSymbol(SymbolTable *table, char *name);
Symbol* searchSymbol(SymbolTable *table, char *name);
void parseInput(FILE *input, SymbolTable *table);
void display(SymbolTable *table);
void generateOutputFile(FILE *input, SymbolTable *table, const char *outputFileName);

int main()
{

// step -1:

    FILE *p1, *p2;
    char c, lex[93];
    int i;

    p1 = fopen("in3.txt", "r");
    p2 = fopen("out1.txt", "w");

    if(!p1)
    {
        printf("\nFile can't be opened!");
    }
    if(!p2)
    {
        printf("\nFile can't be opened!");
    }
    else
    {
        while (fscanf(p1, "%s", lex) != EOF)
        {
            if (strcmp(lex, "[kw") == 0 || strcmp(lex, "[op") == 0 || strcmp(lex, "[sep") == 0 || strcmp(lex, "[par") == 0
                    || strcmp(lex, "[num") == 0 || strcmp(lex, "[unkn") == 0)
            {
                fprintf(p2, "[");
                printf("[");
            }
            else
            {
                fprintf(p2, "%s ", lex);
                printf("%s ", lex);
            }
        }
    }
    printf("\n");

    fclose(p1);
    fclose(p2);

// step-2:

    FILE *inputFile, *outputFile;
    SymbolTable table;
    table.count = 0;

    inputFile = fopen("out1.txt", "r");
    if (!inputFile)
    {
        perror("Error opening input file");
        return -1;
    }

    parseInput(inputFile, &table);
    fclose(inputFile);

    display(&table);

// step-3:

    // Insert a new symbol
    Symbol newSymbol;
    newSymbol.sl_no = 101;
    strcpy(newSymbol.name, "newVar");
    strcpy(newSymbol.id, "var");
    strcpy(newSymbol.data_type, "int");
    strcpy(newSymbol.scope, "global");
    strcpy(newSymbol.value, "10");
    insertOrUpdate(&table, newSymbol);

    // Update an existing symbol
    /*Symbol updatedSymbol;
    updatedSymbol.sl_no = 102;
    strcpy(updatedSymbol.name, "main");
    strcpy(updatedSymbol.id, "func");
    strcpy(updatedSymbol.data_type, "int");
    strcpy(updatedSymbol.scope, "global");
    strcpy(updatedSymbol.value, " ");
    updateSymbol(&table, "main", updatedSymbol);*/

    // Delete a symbol
    deleteSymbol(&table, "varToDelete");

    // Search for a symbol
    Symbol *searchedSymbol = searchSymbol(&table, "z");
    if (searchedSymbol != NULL) {
        printf("\nFound symbol: %s\n", searchedSymbol->name);
    } else {
        printf("\nSymbol not found.\n");
    }

//step-4:

    inputFile = fopen("out1.txt", "r");
    if (!inputFile)
    {
        perror("Error opening input file");
        return -1;
    }

    generateOutputFile(inputFile, &table, "out2.txt");
    fclose(inputFile);

    outputFile = fopen("out2.txt", "r");
    printf("\nOutput File:\n\n");

    while((c = fgetc(outputFile)) != EOF)
    {
        printf("%c", c);
    }

    fclose(outputFile);
    printf("\n");

    return 0;
}

void insertOrUpdate(SymbolTable *table, Symbol symbol)
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, symbol.name) == 0)
        {
            // Symbol with the same name already exists, update its values
            strcpy(table->symbols[i].id, symbol.id);
            strcpy(table->symbols[i].data_type, symbol.data_type);
            strcpy(table->symbols[i].scope, symbol.scope);
            strcpy(table->symbols[i].value, symbol.value);
            return;
        }
    }
    // Symbol with this name doesn't exist, insert it
    table->symbols[table->count++] = symbol;
}

void display(SymbolTable *table) {
    printf("Sl. No.\tName\tId Type\tData Type\tScope\tValue\n");
    FILE *outputFile = fopen("tan5.txt", "w");
    if (!outputFile) {
        perror("Error opening output file");
        return;
    }

    fprintf(outputFile, "Sl. No.\tName\tId Type\tData Type\tScope\tValue\n");

    int sl_no = 1; // Initialize serial number
    for (int i = 0; i < table->count; i++) {
        printf("%d\t%s\t%s\t%s\t%s\t%s\n",
               sl_no,
               table->symbols[i].name,
               table->symbols[i].id,
               table->symbols[i].data_type,
               table->symbols[i].scope,
               table->symbols[i].value);

        fprintf(outputFile, "%d\t%s\t%s\t%s\t%s\t%s\n",
               sl_no,
               table->symbols[i].name,
               table->symbols[i].id,
               table->symbols[i].data_type,
               table->symbols[i].scope,
               table->symbols[i].value);
        sl_no++;
    }

    fclose(outputFile);
}

void parseInput(FILE *input, SymbolTable *table)
{
    char lex[93], type[10], scope[50] = "global";
    int sl_no = 1; // Initialize serial number

    while (fscanf(input, "%s", lex) != EOF)
    {
        if (strcmp(lex, "[float]") == 0 || strcmp(lex, "[double]") == 0 || strcmp(lex, "[int]") == 0 || strcmp(lex, "[char]") == 0)
        {
            strcpy(type, lex + 1);
            type[strlen(type) - 1] = '\0'; // Remove the trailing ']'

            fscanf(input, "%s", lex); // [id
            fscanf(input, "%s", lex); // identifier
            lex[strlen(lex) - 1] = '\0'; // Remove the trailing ']'

            Symbol symbol;
            symbol.sl_no = sl_no++;
            strcpy(symbol.name, lex);
            strcpy(symbol.id, "var");
            strcpy(symbol.data_type, type);
            strcpy(symbol.scope, scope);
            symbol.value[0] = '\0';

            fscanf(input, "%s", lex); // [=] or [;]
            if (strcmp(lex, "[=]") == 0)
            {
                fscanf(input, "%s", symbol.value); // value
                symbol.value[strlen(symbol.value) - 1] = '\0'; // Remove the trailing ']'
                fscanf(input, "%s", lex); // [;]
            }

            insertOrUpdate(table, symbol);
        }
        else if (strcmp(lex, "[id") == 0 || strcmp(lex, "main]") == 0)     // "main" is considered as a function
        {
            fscanf(input, "%s", lex); // function name
            lex[strlen(lex) - 1] = '\0'; // Remove the trailing ']'

            Symbol symbol;
            symbol.sl_no = sl_no++;
            strcpy(symbol.name, lex);
            strcpy(symbol.id, "func");
            strcpy(symbol.data_type, "double"); // Functions by default have void return type
            strcpy(symbol.scope, lex); // Set scope to function name
            symbol.value[0] = '\0';

            insertOrUpdate(table, symbol);
        }
        else if (strcmp(lex, "[return]") == 0)
        {
            fscanf(input, "%s", lex); // [id
            fscanf(input, "%s", lex); // return variable

            // Find the function symbol and update its data type
            for (int i = table->count - 1; i >= 0; i--)
            {
                if (strcmp(table->symbols[i].id, "func") == 0)
                {
                    strcpy(table->symbols[i].data_type, lex);
                    break;
                }
            }
        }
        else if (strcmp(lex, "[}]") == 0)
        {
            strcpy(scope, "global");
        }
    }
}


void updateSymbol(SymbolTable *table, char *name, Symbol newSymbol)
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            table->symbols[i] = newSymbol;
           // printf("\nImplementing functions: \n\n");
           // display(table);
            return;
        }
    }
}

void deleteSymbol(SymbolTable *table, char *name)
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            // Shift symbols to overwrite the deleted one
            for (int j = i; j < table->count - 1; j++)
            {
                table->symbols[j] = table->symbols[j + 1];
            }
            table->count--;
            return;
        }
    }
}

Symbol* searchSymbol(SymbolTable *table, char *name)
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            return &table->symbols[i];
        }
    }
    return NULL; // Symbol not found
}

void generateOutputFile(FILE *input, SymbolTable *table, const char *outputFileName)
{
    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile)
    {
        perror("Error opening output file");
        return;
    }

    char lex[93];
    while (fscanf(input, "%s", lex) != EOF)
    {
        if (strcmp(lex, "[id") == 0)
        {
            fscanf(input, "%s", lex); // identifier
            lex[strlen(lex) - 1] = '\0'; // Remove the trailing ']'

            // Search for the symbol in the symbol table
            Symbol *symbol = searchSymbol(table, lex);
            if (symbol)
            {
                fprintf(outputFile, "[id %d] ", symbol->sl_no);
            }
            else
            {
                fprintf(outputFile, "[id %s] ", lex); // In case the symbol is not found
            }
        }
        else
        {
            fprintf(outputFile, "%s ", lex);
        }
    }

    fclose(outputFile);
}

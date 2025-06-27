#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent an entry in the symbol table
typedef struct {
    int serialNumber;
    char idName[50];
    char idType[50];
} SymbolTableEntry;

// Function prototypes
void insert(SymbolTableEntry table[], int *count, const char *idName, const char *idType, int index);
void update(SymbolTableEntry table[], int count, const char *idName, const char *newIdType);
void deleteEntry(SymbolTableEntry table[], int *count, int index);
void search(SymbolTableEntry table[], int count, const char *idName);
void display(SymbolTableEntry table[], int count);
void loadSymbolTable(SymbolTableEntry table[], int *count);
void saveSymbolTable(SymbolTableEntry table[], int count);

int main() {
    SymbolTableEntry table[100];
    int count = 0;

    // Load the symbol table from the file
    loadSymbolTable(table, &count);

    // Example operations
    insert(table, &count, "test", "var", 5);
    update(table, count, "test", "function");
    ndeleteEntry(table, &count, 3);
    search(table, count, "main");
    saveSymbolTable(table, count);
    display(table, count);

    return 0;
}

// Function to insert a new entry at a given index
void insert(SymbolTableEntry table[], int *count, const char *idName, const char *idType, int index) {
    if (index < 1 || index > *count + 1) {
        printf("Invalid index!\n");
        return;
    }

    for (int i = *count; i >= index; i--) {
        table[i] = table[i - 1];
        table[i].serialNumber = i + 1;
    }

    table[index - 1].serialNumber = index;
    strcpy(table[index - 1].idName, idName);
    strcpy(table[index - 1].idType, idType);

    for (int i = index; i < *count; i++) {
        table[i].serialNumber = i + 1;
    }

    (*count)++;
}

// Function to update an existing entry
void update(SymbolTableEntry table[], int count, const char *idName, const char *newIdType) {
    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].idName, idName) == 0) {
            strcpy(table[i].idType, newIdType);
            printf("Updated entry: %d %s %s\n", table[i].serialNumber, table[i].idName, table[i].idType);
            return;
        }
    }
    printf("Entry with ID name '%s' not found!\n", idName);
}

// Function to delete an entry by index
void deleteEntry(SymbolTableEntry table[], int *count, int index) {
    if (index < 1 || index > *count) {
        printf("Invalid index!\n");
        return;
    }

    for (int i = index; i < *count; i++) {
        table[i - 1] = table[i];
        table[i - 1].serialNumber = i;
    }

    (*count)--;
}

// Function to search for an entry by ID name
void search(SymbolTableEntry table[], int count, const char *idName) {
    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].idName, idName) == 0) {
            printf("Found entry: %d %s %s\n", table[i].serialNumber, table[i].idName, table[i].idType);
            return;
        }
    }
    printf("Entry with ID name '%s' not found!\n", idName);
}

// Function to display the symbol table
void display(SymbolTableEntry table[], int count) {
    printf("The updated Symbol Table:\n");
    for (int i = 0; i < count; i++) {
        printf("%d %s %s\n", table[i].serialNumber, table[i].idName, table[i].idType);
    }
}

// Function to load the symbol table from a file into the array
void loadSymbolTable(SymbolTableEntry table[], int *count) {
    FILE *file = fopen("symbolTable.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    while (fscanf(file, "%d %s %s", &table[*count].serialNumber, table[*count].idName, table[*count].idType) == 3) {
        (*count)++;
    }

    fclose(file);
}

// Function to save the symbol table to the file
void saveSymbolTable(SymbolTableEntry table[], int count) {
    FILE *file = fopen("symbolTable.txt", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s\n", table[i].serialNumber, table[i].idName, table[i].idType);
    }

    fclose(file);
}

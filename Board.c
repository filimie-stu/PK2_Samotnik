#include "Board.h"
#include "Vector2D.h"
#include "Field.h"
#include <stdlib.h>
#include <stdio.h>


static int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile);
static int private_loadFields(Board *incompleteSelf, FILE *sourceFile);

Board* Board_newFromFile(const char* relativePath)
{
    Board *created = (Board *)malloc(sizeof(Board));
    created->activeField = NULL;
    created->tokenCount = 0;

    FILE *file;
    if (fopen_s(&file, relativePath, "r") != 0)
    {
        perror("Failed to open specified file. Aborting...\n");
        return NULL;
    }
    if (private_loadDimensions(created, file) != 0)
    {
        perror("Failed to read board dimensions. Aborting...\n");
        return NULL;
    }
    if (private_loadFields(created, file) != 0)
    {
        perror("An error occurred while creating game board.\n");
        return NULL;
    }

    fclose(file);
    return created;
}

void Board_destroy(Board* self)
{
    for (int i = 0; i < self->dimensions.y; i++)
        free(self->fields[i]); // free the subarrays
    free(self->fields);         // free the array of subarrays
    free(self);
}


int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile)
{
    if (fscanf(sourceFile, "%d %d ", &incompleteSelf->dimensions.y, &incompleteSelf->dimensions.x) < 2)
        return 1;
    return 0;
}

int private_loadFields(Board *incompleteSelf, FILE *sourceFile)
{
    int rows = incompleteSelf->dimensions.y;
    int cols = incompleteSelf->dimensions.x;
    
    incompleteSelf->fields = (Field **)malloc(sizeof(Field *) * rows);
    for (int i = 0; i < rows; i++)
        incompleteSelf->fields[i] = (Field *)malloc(sizeof(Field) * cols);
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char c;
            if (fscanf(sourceFile, "%c ", &c) != 1)
                return 1;

            incompleteSelf->fields[i][j] = Field_create(i, j, FieldType_fromChar(c));
            if (incompleteSelf->fields[i][j].contents == REGULAR_TOKEN || incompleteSelf->fields[i][j].contents == ACTIVE_TOKEN)
                incompleteSelf->tokenCount++;

        }
    }
    return 0;
}

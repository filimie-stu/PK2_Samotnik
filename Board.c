#include "JumpArgs.h"
#include "ActivateArgs.h"
#include "Board.h"
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


static int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile);
static int private_loadFields(Board *incompleteSelf, FILE *sourceFile);
static ClickResult private_determineAction(Board* self, Vector2D clickCoords);
static Field* private_fieldAt(Board* self, Vector2D at);
static int private_canActivate(Board* self, Vector2D clickedCoords);
static int private_canJump(Board* self, Vector2D clickedCoords);
static void private_takedownAt(Board* self, Vector2D at);
static void private_transferActiveToken(Board* self, Vector2D dst);
static void private_jump(Board *self, Vector2D dstCoords);

Board* Board_newFromFile(const char* relativePath)
{
    Board *created = (Board *)malloc(sizeof(Board));
    created->activeField = NULL;
    created->tokenCount = 0;
    created->observable = Observable_new(created);

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

void private_activateAt(Board* self, Vector2D at)
{
    ActivateArgs args;

    if (self->activeField)
    {
        self->activeField->contents = REGULAR_TOKEN;
        args.previouslyActive = self->activeField->coords;
    }
    else
    {
        args.previouslyActive.x = -1;
        args.previouslyActive.y = -1;
    }
    
    self->activeField = private_fieldAt(self, at);
    self->activeField->contents = ACTIVE_TOKEN;

    args.activated = self->activeField->coords;
    Observable_notifyObservers(self->observable, "activate_token", &args);
}
ClickResult Board_clickField(Board* self, Vector2D coords)
{
    ClickResult action = private_determineAction(self, coords);
    switch (action)
    {
    case ACTIVATE_TOKEN:
        private_activateAt(self, coords);
        break;
    
    case JUMP:
        private_jump(self, coords);
        break;
    default:
        break;
    }

    return action;
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

Field* private_fieldAt(Board* self, Vector2D at)
{
    return &self->fields[at.x][at.y];
}

ClickResult private_determineAction(Board* self, Vector2D clickCoords )
{
    if (private_canJump(self, clickCoords))
        return JUMP;
    
    if (private_canActivate(self, clickCoords))
        return ACTIVATE_TOKEN;

    return NO_ACTION;
}

int private_canActivate(Board* self, Vector2D clickedCoords)
{
    if (private_fieldAt(self, clickedCoords)->contents == REGULAR_TOKEN)
        return 1;
    return 0;
}

int private_canJump(Board* self, Vector2D clickedCoords)
{
    if (!self->activeField)
        return 0;

    int destinationEmpty = 
        private_fieldAt(self, clickedCoords)->contents == EMPTY;
    int fieldsInLine = 
        self->activeField->coords.x == clickedCoords.x || 
        self->activeField->coords.y == clickedCoords.y;
    int rightDistance = 
        abs(self->activeField->coords.x - clickedCoords.x) == 2 ||
        abs(self->activeField->coords.y - clickedCoords.y) == 2; 
    
    Vector2D attackedCoords = Vector2D_calculateMidpoint(self->activeField->coords, clickedCoords);
    int tokenBetween = 
        private_fieldAt(self, attackedCoords)->contents == REGULAR_TOKEN;

    return destinationEmpty * fieldsInLine * rightDistance * tokenBetween;
}
void private_jump(Board *self, Vector2D dstCoords)
{
    Vector2D attackedCoords = Vector2D_calculateMidpoint(self->activeField->coords, dstCoords);
    JumpArgs eventArgs = { self->activeField->coords, attackedCoords, dstCoords };
    Observable_notifyObservers(self->observable, "jump", &eventArgs);

    private_takedownAt(self, attackedCoords);
    private_transferActiveToken(self, dstCoords);

}

void private_takedownAt(Board* self, Vector2D at)
{
    private_fieldAt(self, at)->contents = EMPTY;
}
void private_transferActiveToken(Board* self, Vector2D dst)
{
    self->activeField->contents = EMPTY;
    self->activeField = NULL;
    private_fieldAt(self, dst)->contents = REGULAR_TOKEN;
}


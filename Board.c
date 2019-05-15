#include "ActivateTokenArgs.h"
#include "JumpArgs.h"
#include "Board.h"
#include "Common.h"
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

static Field *private_getNeighbourOf(Board *self, Field *field, Direction dir);
static void private_getJumpableFields(Board *self, Field *from, Field *out_fields[4]);
static int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile);
static int private_loadFields(Board *incompleteSelf, FILE *sourceFile);
static Field *private_fieldAt(Board *self, Vector2D at);
static int private_isDeadEndState(Board *self);
static int private_jumpsPossibleFrom(Board *self, Field *from);
static void private_applyJump(Board *self, JumpInfo jump);

Board *Board_newFromFile(const char *relativePath)
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

void Board_destroy(Board *self)
{
    for (int i = 0; i < self->dimensions.y; i++)
        free(self->fields[i]); // free the subarrays
    free(self->fields);        // free the array of subarrays
    free(self);
}

int Board_tryJump(Board *self, Vector2D from, Vector2D to)
{
    Field *fromField = private_fieldAt(self, from);
    Field *toField = private_fieldAt(self, to);

    if (!fromField || !toField)
    {
        return 0;
    }
    if (fromField->contents != ACTIVE_TOKEN && fromField->contents != REGULAR_TOKEN)
    {
        return 0;
    }

    Field *spots[4];
    private_getJumpableFields(self, fromField, spots);

    for (Direction dir = 0; dir < 4; dir++)
    {
        if (spots[dir] && spots[dir] == toField)
        {
            Field *attackedField = private_getNeighbourOf(self, fromField, dir);
            assert(attackedField != NULL && "This should've been already null-checked by 'getJumpableFields'");

            JumpInfo jump = {from, attackedField->coords, to};
            private_applyJump(self, jump);
            return 1;
        }
    }
    return 0;
}

int Board_tryActivate(Board *self, Vector2D at)
{
    Field *activatedField = private_fieldAt(self, at);
    if (!activatedField)
        return 0;

    if (activatedField->contents != REGULAR_TOKEN)
        return 0;

    Field *jumpSpots[4];
    private_getJumpableFields(self, activatedField, jumpSpots);

    ActivateTokenArgs activationArgs;
    activationArgs.activeCoords = at;
    for (Direction dir = 0; dir < 4; dir++)
    {
        if (jumpSpots[dir])
        {
            activationArgs.jumpSpots[dir] = jumpSpots[dir]->coords;
        }
        else
        {
            activationArgs.jumpSpots[dir] = Vector2D_create(-1, -1);
        }
    }
    Observable_notifyObservers(self->observable, "activate", &activationArgs);
    return 1;
}



void private_applyJump(Board *self, JumpInfo jump)
{
    private_fieldAt(self, jump.from)->contents = EMPTY;
    private_fieldAt(self, jump.through)->contents = EMPTY;
    private_fieldAt(self, jump.to)->contents = REGULAR_TOKEN;

    Observable_notifyObservers(self->observable, "jump", &jump);

    if (private_isDeadEndState(self))
    {
        Observable_notifyObservers(self->observable, "dead_end", NULL);
    }
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
Field *private_fieldAt(Board *self, Vector2D at)
{
    if (self->dimensions.x <= at.x ||
        self->dimensions.y <= at.y ||
        at.x < 0 ||
        at.y < 0)
        return NULL;

    return &self->fields[at.x][at.y];
}
Field *private_getNeighbourOf(Board *self, Field *field, Direction dir)
{
    assert(field != NULL);

    Vector2D coords = field->coords;
    switch (dir)
    {
    case UP:
        return private_fieldAt(self, Vector2D_create(coords.x - 1, coords.y));

    case DOWN:
        return private_fieldAt(self, Vector2D_create(coords.x + 1, coords.y));

    case LEFT:
        return private_fieldAt(self, Vector2D_create(coords.x, coords.y - 1));

    case RIGHT:
        return private_fieldAt(self, Vector2D_create(coords.x, coords.y + 1));
    default:
        assert(0 && "We should never stop here.\n");
        return NULL;
    }
}
void private_getJumpableFields(Board *self, Field *from, Field *out_fields[4])
{
    assert(from != NULL);

    for (Direction dir = 0; dir < 4; dir++)
    {
        Field *attackedField = private_getNeighbourOf(self, from, dir);
        if (attackedField != NULL && attackedField->contents == REGULAR_TOKEN)
        {
            Field *jumpDestination = private_getNeighbourOf(self, attackedField, dir);
            if (jumpDestination != NULL && jumpDestination->contents == EMPTY)
            {
                out_fields[dir] = jumpDestination;
                continue;
            }
        }
        out_fields[dir] = NULL;
    }
}
int private_jumpsPossibleFrom(Board *self, Field *from)
{
    if (from->contents != REGULAR_TOKEN)
    {
        return 0;
    }

    Field *jumpableFields[4];
    private_getJumpableFields(self, from, jumpableFields);

    for (int i = 0; i < 4; i++)
    {
        if (jumpableFields[i] != NULL)
        {
            return 1;
        }
    }

    return 0;
}
int private_isDeadEndState(Board *self)
{
    for (int i = 0; i < self->dimensions.x; i++)
    {
        for (int j = 0; j < self->dimensions.y; j++)
        {
            Field *field = private_fieldAt(self, Vector2D_create(i, j));
            if (private_jumpsPossibleFrom(self, field))
                return 0;
        }
    }
    return 1;
}

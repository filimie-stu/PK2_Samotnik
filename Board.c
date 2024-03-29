#include "ActivateTokenArgs.h"
#include "Board.h"
#include "Common.h"
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/**
    \brief Reprezentuje planszę do gry. Odpowiada za operacje na niej wykonywane, czyli m. in.: przesuwanie pionków, aktywację pól, cofanie ruchów.

    Dokładnym zakresem odpowiedzialności klasy Board (poza udostępnianiem kilku akcesorów) 
    są funkcjonalności związane z: przesuwaniem pionków (tryJump), aktywacją pól (tryActivate), cofanie skoków (rollbackJump),
    oraz sprawdzanie czy gra nie utkwiła w martwym punkcie.
*/
typedef struct board 
{
    IBoard* iBoard;         //!< implementacja interfejsu iBoard
    Vector2D dimensions;    //!< wymiary plaszy
    Field** fields;         //!< dynamicznie alokowana dwuwymiarowa tablica pól
    Field* activeField;     //!< wskaźnik na aktualnie aktywne pole
    int tokenCount;         //!< licznik pionków na planszy
} Board;



static Field *private_getNeighbourOf(Board *self, Field *field, Direction dir);
static void private_getJumpableFields(Board *self, Field *from, Field *out_fields[4]);
static int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile);
static int private_loadFields(Board *incompleteSelf, FILE *sourceFile);
static Field *private_fieldAt(Board *self, Vector2D at);
static int private_jumpsPossibleFrom(Board *self, Field *from);
static void private_applyJump(Board *self, JumpInfo jump);
static int private_wrapper_tryJump(void *vSelf, Vector2D from, Vector2D to, JumpInfo *out_jumpData);
static int private_wrapper_tryActivate(void *vSelf, Vector2D at);
static void private_wrapper_rollbackJump(void *vSelf, JumpInfo jumpData);
static void private_wrapper_destroy(void *vSelf);
static FieldType private_wrapper_getFieldAt(void *vSelf, Vector2D at);
static Vector2D private_wrapper_getDimensions(void *vSelf);
static int private_wrapper_countTokens(void *vSelf);
static int private_wrapper_isDeadEnd(void *vSelf);



void Board_destroy(Board *self)
{
    IBoard_destroy(self->iBoard, 0);
    self->iBoard = NULL;

    for (int i = 0; i < self->dimensions.y; i++)
        free(self->fields[i]); // free the subarrays
    free(self->fields);        // free the array of subarrays
    free(self);
}

int private_wrapper_isDeadEnd(void* vSelf)
{
    return Board_isDeadEnd((Board*)vSelf);
}

int private_wrapper_countTokens(void *vSelf)
{
    return Board_countTokens((Board *)vSelf);
}

Observable *Board_asObservable(Board *self)
{
    return IBoard_asObservable(self->iBoard);
}
int Board_countTokens(Board *self)
{
    int count = 0;
    for (int i = 0; i < self->dimensions.x; i++)
    {
        for (int j = 0; j < self->dimensions.y; j++)
        {
            Vector2D at = {i, j};
            if (Board_getFieldAt(self, at) == REGULAR_TOKEN || Board_getFieldAt(self, at) == ACTIVE_TOKEN)
            {
                count++;
            }
        }
    }
    return count;
}
FieldType private_wrapper_getFieldAt(void *vSelf, Vector2D at)
{
    return Board_getFieldAt((Board *)vSelf, at);
}
Vector2D private_wrapper_getDimensions(void *vSelf)
{
    return Board_getDimensions((Board *)vSelf);
}

Vector2D Board_getDimensions(Board *self)
{
    return self->dimensions;
}
FieldType Board_getFieldAt(Board *self, Vector2D at)
{
    return private_fieldAt(self, at)->contents;
}
int private_wrapper_tryJump(void *vSelf, Vector2D from, Vector2D to, JumpInfo *out_jumpData)
{
    return Board_tryJump((Board *)vSelf, from, to, out_jumpData);
}
int private_wrapper_tryActivate(void *vSelf, Vector2D at)
{
    return Board_tryActivate((Board *)vSelf, at);
}
void private_wrapper_rollbackJump(void *vSelf, JumpInfo jumpData)
{
    return Board_rollbackJump((Board *)vSelf, jumpData);
}
void private_wrapper_destroy(void *vSelf)
{
    Board_destroy((Board *)vSelf);
}

IBoard *Board_asIBoard(Board *self)
{
    return self->iBoard;
}
void Board_rollbackJump(Board *self, JumpInfo jump)
{
    // validate it!

    private_fieldAt(self, jump.from)->contents = REGULAR_TOKEN;
    private_fieldAt(self, jump.through)->contents = REGULAR_TOKEN;
    private_fieldAt(self, jump.to)->contents = EMPTY;

    Observable_notifyObservers(Board_asObservable(self), "rollback", &jump);
}
Board *Board_newFromFile(const char *relativePath)
{
    Board *created = (Board *)malloc(sizeof(Board));
    created->iBoard = IBoard_new(
        created,
        private_wrapper_destroy,
        private_wrapper_tryJump,
        private_wrapper_tryActivate,
        private_wrapper_rollbackJump,
        private_wrapper_getFieldAt,
        private_wrapper_getDimensions,
        private_wrapper_countTokens,
        private_wrapper_isDeadEnd);
    created->activeField = NULL;
    created->tokenCount = 0;

    FILE *file = fopen(relativePath, "r");
    if (file == NULL)
    {
        perror("Failed to open specified board file. \n");
        exit(1);
        // return NULL;
    }
    if (private_loadDimensions(created, file) != 0)
    {
        perror("Failed to parse board dimensions. \n");
        exit(1);
        // return NULL;
    }
    if (private_loadFields(created, file) != 0)
    {
        perror("An error occurred while creating game board.\n");
        exit(1);
        // return NULL;
    }

    fclose(file);
    return created;
}


int Board_tryJump(Board *self, Vector2D from, Vector2D to, JumpInfo *out_jumpData)
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
            *out_jumpData = jump;
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
    Observable_notifyObservers(Board_asObservable(self), "activate", &activationArgs);
    return 1;
}
int Board_isDeadEnd(Board *self)
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
void private_applyJump(Board *self, JumpInfo jump)
{
    private_fieldAt(self, jump.from)->contents = EMPTY;
    private_fieldAt(self, jump.through)->contents = EMPTY;
    private_fieldAt(self, jump.to)->contents = REGULAR_TOKEN;

    Observable_notifyObservers(Board_asObservable(self), "jump", &jump);
}
int private_loadDimensions(Board *incompleteSelf, FILE *sourceFile)
{
    if (fscanf(sourceFile, "%d %d ", &incompleteSelf->dimensions.y, &incompleteSelf->dimensions.x) < 2)
        return 1;
    if (incompleteSelf->dimensions.y < 5 || incompleteSelf->dimensions.x < 5)
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

    int i = 0;
    char* row = (char*)calloc(cols + 1, 1);
    for (; i < rows; i++)
    {
        row[cols-1] = '\0';
        row[cols] = '\0';
        fscanf(sourceFile, "%s ", row);
        if (row[cols] != '\0' || row[cols-1] == '\0')
            return 1;

        for (int j = 0; j < cols; j++)
        {
            incompleteSelf->fields[i][j] = Field_create(i, j, FieldType_fromChar(row[j]));
            if (incompleteSelf->fields[i][j].contents == REGULAR_TOKEN || incompleteSelf->fields[i][j].contents == ACTIVE_TOKEN)
                incompleteSelf->tokenCount++;                
        }
    }
    free(row);

    if (i != rows)
        return 1;
    if (incompleteSelf->tokenCount < 1)
        return 1;
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

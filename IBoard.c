#include "Observable.h"
#include "IBoard.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct i_board
{
    void *implObject;
    void(*destroyOverride)(void* implObject);
    int (*tryJumpOverride)(void *implObject, Vector2D from, Vector2D to);
    int (*tryActivateOverride)(void *implObject, Vector2D at);
    void (*rollbackJumpOverride)(void *implObject, JumpInfo jumpData);
    

    Observable* observable;

} IBoard;
Observable* IBoard_asObservable(IBoard* self)
{
    return self->observable;
}
IBoard *IBoard_new(
    void *implObject,
    void(*destroyOverride)(void* implObject),
    int (*tryJumpOverride)(void *implObject, Vector2D from, Vector2D to),
    int (*tryActivateOverride)(void *implObject, Vector2D at),
    void (*rollbackJumpOverride)(void *implObjet, JumpInfo jumpData)
    
)
{
    if (!implObject || !destroyOverride || !tryJumpOverride || !tryActivateOverride || !rollbackJumpOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IBoard *created = (IBoard *)malloc(sizeof(IBoard));
    created->implObject = implObject;
    created->destroyOverride = destroyOverride;
    created->tryJumpOverride = tryJumpOverride;
    created->tryActivateOverride = tryActivateOverride;
    created->rollbackJumpOverride = rollbackJumpOverride;
    created->observable = Observable_new(created);

    return created;
}
void IBoard_destroy(IBoard *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implObject);
    }
    else
    {
        free(self);
    }
}
int IBoard_tryJump(IBoard *self, Vector2D from, Vector2D to)
{
    return self->tryJumpOverride(self->implObject, from, to);
}
int IBoard_tryActivate(IBoard *self, Vector2D at)
{
    return self->tryActivateOverride(self->implObject, at);
}
void IBoard_rollbackJump(IBoard *self, JumpInfo jumpData)
{
    self->rollbackJumpOverride(self->implObject, jumpData);
}
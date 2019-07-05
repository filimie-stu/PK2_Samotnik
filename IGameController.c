#include "IGameController.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * \brief Interfejs implementowany przez kontroler gry (GameController).
 * \sa game_controller
*/
typedef struct i_game_controller
{
    void *implementationObject;
    void (*destroyOverride)(void *implObject);
    void (*mainMenuOverride)(void *implObject);
    void (*prepareForExitOverride)(void *implObject);
    void (*beginMatchOverride)(void *implObject, NewGameArgs settings);
    void (*restartGameOverride)(void *implObject);
    void (*continueMatchOverride)(void *implObject);
    void (*endMatchOverride)(void *implObject);
    void (*rollbackOverride)(void *implObjet);
    void (*jumpOverride)(void *implObject, Vector2D from, Vector2D to);
    void (*activateOverride)(void *implObject, Vector2D at);

} IGameController;

IGameController *IGameController_new(
    void *implementationObject,
    void (*destroyOverride)(void *implObject),
    void (*mainMenuOverride)(void *implObject),
    void (*prepareForExitOverride)(void *implObject),
    void (*beginMatchOverride)(void *implObject, NewGameArgs settings),
    void (*restartGameOverride)(void *implObject),
    void (*continueMatchOverride)(void *implObject),
    void (*endMatchOverride)(void *implObject),
    void (*rollbackOverride)(void *implObjet),
    void (*jumpOverride)(void *implObject, Vector2D from, Vector2D to),
    void (*activateOverride)(void *implObject, Vector2D at))
{
    if (
        !destroyOverride || !mainMenuOverride || !prepareForExitOverride ||
        !beginMatchOverride || !restartGameOverride || !continueMatchOverride ||
        !endMatchOverride || !rollbackOverride || !jumpOverride ||
        !activateOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IGameController *created = (IGameController *)malloc(sizeof(IGameController));
    created->implementationObject = implementationObject;
    created->destroyOverride = destroyOverride;
    created->mainMenuOverride = mainMenuOverride;
    created->prepareForExitOverride = prepareForExitOverride;
    created->beginMatchOverride = beginMatchOverride;
    created->restartGameOverride = restartGameOverride;
    created->continueMatchOverride = continueMatchOverride;
    created->endMatchOverride = endMatchOverride;
    created->rollbackOverride = rollbackOverride;
    created->jumpOverride = jumpOverride;
    created->activateOverride = activateOverride;

    return created;
}
void IGameController_destroy(IGameController *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        free(self);
    }
}
void IGameController_mainMenu(IGameController *self)
{
    self->mainMenuOverride(self->implementationObject);
}
void IGameController_prepareForExit(IGameController *self)
{
    self->prepareForExitOverride(self->implementationObject);
}
void IGameController_beginMatch(IGameController *self, NewGameArgs settings)
{
    self->beginMatchOverride(self->implementationObject, settings);
}
void IGameController_restartGame(IGameController *self)
{
    self->restartGameOverride(self->implementationObject);
}
void IGameController_continueMatch(IGameController *self)
{
    self->continueMatchOverride(self->implementationObject);
}
void IGameController_endMatch(IGameController *self)
{
    self->endMatchOverride(self->implementationObject);
}
void IGameController_rollback(IGameController *self)
{
    self->rollbackOverride(self->implementationObject);
}
void IGameController_jump(IGameController *self, Vector2D from, Vector2D to)
{
    self->jumpOverride(self->implementationObject, from, to);
}
void IGameController_activate(IGameController *self, Vector2D at)
{
    self->activateOverride(self->implementationObject, at);
}

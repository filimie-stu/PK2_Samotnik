#pragma once
#include "NewGameArgs.h"
#include "Vector2D.h"

typedef struct i_game_controller IGameController;

IGameController* IGameController_new(
    void* implementationObject,
    void(*destroyOverride)(void* implObject),
    void(*mainMenuOverride)(void* implObject),
    void(*prepareForExitOverride)(void* implObject),
    void(*beginMatchOverride)(void* implObject, NewGameArgs settings),
    void(*restartGameOverride)(void* implObject),
    void(*continueMatchOverride)(void* implObject),
    void(*endMatchOverride)(void* implObject),
    void(*rollbackOverride)(void* implObjet),
    void(*jumpOverride)(void* implObject, Vector2D from, Vector2D to),
    void(*activateOverride)(void* implObject, Vector2D at)
);
void IGameController_destroy(IGameController* self, int destroyDerivedTypes);
void IGameController_mainMenu(IGameController* self);
void IGameController_prepareForExit(IGameController* self);
void IGameController_beginMatch(IGameController* self, NewGameArgs settings);
void IGameController_restartGame(IGameController* self);
void IGameController_continueMatch(IGameController* self);
void IGameController_endMatch(IGameController* self);
void IGameController_rollback(IGameController* self);
void IGameController_jump(IGameController* self, Vector2D from, Vector2D to);
void IGameController_activate(IGameController* self, Vector2D at);

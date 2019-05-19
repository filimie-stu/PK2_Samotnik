#pragma once
#include "Vector2D.h"
#include "NewGameArgs.h"
#include "IGameController.h"
typedef struct game_controller GameController;


IGameController* GameController_asIGameController(GameController* self);
GameController* GameController_new();
void GameController_destroy(GameController* self);
void GameController_mainMenu(GameController* self);
void GameController_prepareForExit(GameController* self);

void GameController_beginMatch(GameController* self, NewGameArgs settings);
void GameController_restartGame(GameController* self);
void GameController_continueMatch(GameController* self);
void GameController_endMatch(GameController* self);

void GameController_rollback(GameController* self);
void GameController_jump(GameController* self, Vector2D from, Vector2D to);
void GameController_activate(GameController* self, Vector2D at);


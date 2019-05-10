#pragma once

typedef struct game_controller GameController;

GameController* GameController_new();
void GameController_destroy(GameController* self);
void GameController_mainMenu(GameController* self);
void GameController_prepareForExit(GameController* self);
void GameController_beginMatch(GameController* self);
void GameController_restartGame(GameController* self);
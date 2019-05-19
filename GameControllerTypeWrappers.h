#pragma once
#include "GameController.h"
#include "NewGameArgs.h"
#include "Vector2D.h"

void private_wrapper_destroy(void *vSelf);
void private_wrapper_mainMenu(void *vSelf);
void private_wrapper_prepareForExit(void *vSelf);
void private_wrapper_beginMatch(void *vSelf, NewGameArgs settings);
void private_wrapper_restartGame(void *vSelf);
void private_wrapper_continueMatch(void *vSelf);
void private_wrapper_endMatch(void *vSelf);
void private_wrapper_rollback(void *vSelf);
void private_wrapper_jump(void *vSelf, Vector2D from, Vector2D to);
void private_wrapper_activate(void *vSelf, Vector2D at);

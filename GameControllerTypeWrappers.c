#include "GameControllerTypeWrappers.h"

void private_wrapper_destroy(void *vSelf)
{
    GameController_destroy((GameController *)vSelf);
}
void private_wrapper_mainMenu(void *vSelf)
{
    GameController_mainMenu((GameController *)vSelf);
}
void private_wrapper_prepareForExit(void *vSelf)
{
    GameController_prepareForExit((GameController *)vSelf);
}
void private_wrapper_beginMatch(void *vSelf, NewGameArgs settings)
{
    GameController_beginMatch((GameController *)vSelf, settings);
}
void private_wrapper_restartGame(void *vSelf)
{
    GameController_restartGame((GameController *)vSelf);
}
void private_wrapper_continueMatch(void *vSelf)
{
    GameController_continueMatch((GameController *)vSelf);
}
void private_wrapper_endMatch(void *vSelf)
{
    GameController_endMatch((GameController *)vSelf);
}
void private_wrapper_rollback(void *vSelf)
{
    GameController_rollback((GameController *)vSelf);
}
void private_wrapper_jump(void *vSelf, Vector2D from, Vector2D to)
{
    GameController_jump((GameController *)vSelf, from, to);
}
void private_wrapper_activate(void *vSelf, Vector2D at)
{
    GameController_activate((GameController *)vSelf, at);
}

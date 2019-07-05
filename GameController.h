#pragma once
#include "Vector2D.h"
#include "NewGameArgs.h"
#include "IGameController.h"
typedef struct game_controller GameController;

/** \memberof game_controller */
IGameController *GameController_asIGameController(GameController *self);
/** \memberof game_controller */
GameController *GameController_new();

/** \memberof game_controller */
void GameController_destroy(GameController *self);

/** 
 * \memberof game_controller 
 * \brief Wyświetla menu główne.
 * Jeżeli uprzednio rozpoczęta została rozgrywka, jej widok zostaje dezaktywowany, ustępując miejsca widokowi głównego menu.
 * */
void GameController_mainMenu(GameController *self);

/** 
 * \memberof game_controller 
 * \brief Wykonuje czynności niezbędne dla poprawnego zakończenia pracy programu.
*/
void GameController_prepareForExit(GameController *self);

/**
 * \memberof game_controller
 * \brief Rozpoczyna nową grę.
 * Jeżeli gra została uprzednio rozpoczęta, finalizuje obiekty poprzedniej sesji, a w ich miejsce tworzy nowe.
 *  
 * \param settings ustawienia niezbędne do rozpoczęcia nowej gry. 
 * */
void GameController_beginMatch(GameController *self, NewGameArgs settings);

/** 
 * \memberof game_controller 
 * \brief Finalizuje obiekty aktualnej sesji i rozpoczyna nową grę.
*/
void GameController_restartGame(GameController *self);

/** 
 * \memberof game_controller 
 * \brief Kontynuuje rozgrywkę przerwaną przez wyjście do menu.
*/
void GameController_continueMatch(GameController *self);

/** 
 * \memberof game_controller 
 * \brief Finalizuje obiekty aktualnej sesji.
*/
void GameController_endMatch(GameController *self);

/** 
 * \memberof game_controller 
 * \brief Wykonuje żądanie cofnięcia ruchu, o ile to możliwe.
*/
void GameController_rollback(GameController *self);

/**
 * \memberof game_controller 
 * \brief Wykonuje żądanie skoku pionkiem, o ile to możliwe. 
*/
void GameController_jump(GameController *self, Vector2D from, Vector2D to);

/**
 * \memberof game_controller
 * \brief Wykonuje żądanie aktywacji pola, o ile to możliwe.
*/
void GameController_activate(GameController *self, Vector2D at);

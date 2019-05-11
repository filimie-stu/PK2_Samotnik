#include "GameController.h"
#include "MainMenuView.h"
#include "MatchView.h"
#include "Board.h"
#include <stdlib.h>


typedef struct game_controller
{
    Board* board;
    MatchView* currentMatchView;
    MainMenuView* currentMainMenuView;
} GameController;

GameController* GameController_new()
{
    GameController* created = (GameController*)malloc(sizeof(GameController));
    created->currentMainMenuView = NULL;
    created->currentMatchView = NULL;
    created->board = NULL;
    return created;
}
void GameController_destroy(GameController* self)
{
    if (self->currentMatchView)
    {
        MatchView_destroy(self->currentMatchView);
    }
    if (self->currentMainMenuView)
    {
        MainMenuView_destroy(self->currentMainMenuView);
    }
    free(self);
}
void GameController_mainMenu(GameController* self)
{
    if (self->currentMatchView)
        MatchView_hide(self->currentMatchView);

    self->currentMainMenuView = MainMenuView_new(self);
    MainMenuView_display(self->currentMainMenuView);
}

void GameController_prepareForExit(GameController* self)
{
    //...
}

void GameController_restartGame(GameController* self)
{
    //...
}

void GameController_beginMatch(GameController* self)
{
    if (self->currentMainMenuView)
    {
        MainMenuView_hide(self->currentMainMenuView);
    }
    if (self->currentMatchView)
    {
        MatchView_destroy(self->currentMatchView);
    }
    if (self->board)
    {
        Board_destroy(self->board);
    }
    self->board = Board_newFromFile("data/board.txt");
    self->currentMatchView = MatchView_new(self, self->board);
    MatchView_display(self->currentMatchView);
}
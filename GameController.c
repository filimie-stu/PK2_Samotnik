#include "GameController.h"
#include "MainMenuView.h"
#include "MatchView.h"
#include "Board.h"
#include <stdlib.h>
#include <stdio.h>


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

    if (!self->currentMainMenuView)
        self->currentMainMenuView = MainMenuView_new(self);
    
    MainMenuView_display(self->currentMainMenuView);
}

void GameController_prepareForExit(GameController* self)
{
    //...
}

void GameController_restartGame(GameController* self)
{
    printf("restarting\n");
    
    Board_destroy(self->board);
    self->board = Board_newFromFile("data/board.txt");
    
    MatchView_destroy(self->currentMatchView);
    self->currentMatchView = MatchView_new(self, self->board);    
    
    MatchView_display(self->currentMatchView);
}

int private_gameGoing(GameController* self)
{
    if (self->board)
        return 1;
    
    return 0;
}

void GameController_beginMatch(GameController* self)
{
    if (self->currentMainMenuView)
    {
        MainMenuView_hide(self->currentMainMenuView);
    }
    if (private_gameGoing(self))
    {
        Board_destroy(self->board);
    }

    self->board = Board_newFromFile("data/board.txt");
    self->currentMatchView = MatchView_new(self, self->board);
    MatchView_display(self->currentMatchView);
}

void GameController_clickBoard(GameController* self, Vector2D coords)
{
    Board_clickField(self->board, coords);
}
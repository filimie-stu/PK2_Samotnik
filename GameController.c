#include "GameController.h"
#include "MainMenuView.h"
#include "Observer.h"
#include "MatchView.h"
#include "GameOverView.h"
#include "Board.h"
#include "Score.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct game_controller
{
    Board* board;
    Score* score;

    Observer* observingBoard;
    MatchView* currentMatchView;
    MainMenuView* currentMainMenuView;
    GameOverView* gameOverView;
} GameController;

GameController* GameController_new()
{
    GameController* created = (GameController*)malloc(sizeof(GameController));
    created->observingBoard = NULL;
    created->currentMainMenuView = NULL;
    created->currentMatchView = NULL;
    created->gameOverView = NULL;
    created->board = NULL;
    created->score = NULL;
    return created;
}

void GameController_continueMatch(GameController* self)
{
    if (!self->currentMatchView)
    {
        perror("Error: no ongoing match to continue.\n");
        return;
    }

    MainMenuView_hide(self->currentMainMenuView);
    MatchView_display(self->currentMatchView);
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
    if (!self->currentMainMenuView)
    {
        self->currentMainMenuView = MainMenuView_new(self);
    }


    MainMenuView_display(self->currentMainMenuView);

    if (self->currentMatchView)
    {
        MatchView_hide(self->currentMatchView);
        MainMenuView_displayContinueButton(self->currentMainMenuView);
    } 
    else
    {
        MainMenuView_hideContinueButton(self->currentMainMenuView);
    }

}

void GameController_prepareForExit(GameController* self)
{
    //...
}
static void private_gameOver(GameController* self);
void private_gameOver(GameController* self)
{
    printf("Game over\n");
}
static void private_recieveSignal(void* vSelf, const char* signalID, void* signalArgs);

void private_recieveSignal(void* vSelf, const char* signalID, void* signalArgs)
{
    if (strncmp(signalID, "dead_end", strlen(signalID)) == 0)
    {
        private_gameOver((GameController*)vSelf);        
    }
}

void GameController_restartGame(GameController* self)
{
    printf("restarting\n");
    
    Board_destroy(self->board);
    Observer_dispose(self->observingBoard);
    self->board = Board_newFromFile("data/board.txt");
    self->observingBoard = Observer_new(self, private_recieveSignal, self->board->observable);

    Score_destroy(self->score);
    self->score = Score_new();

    MatchView_destroy(self->currentMatchView);
    self->currentMatchView = MatchView_new(self, self->board, self->score);    
    
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
        Score_destroy(self->score);
        Observer_dispose(self->observingBoard);
    }

    self->board = Board_newFromFile("data/board.txt");
    self->observingBoard = Observer_new(self, private_recieveSignal, self->board->observable);
    self->score = Score_new();
    self->currentMatchView = MatchView_new(self, self->board, self->score);
    MatchView_display(self->currentMatchView);
}

void GameController_clickBoard(GameController* self, Vector2D coords)
{
    ClickResult clickResult = Board_clickField(self->board, coords);
    if (clickResult == JUMP)
    {
        Score_increment(self->score);
    }
    
}
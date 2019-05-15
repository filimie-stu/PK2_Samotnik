#include "GameController.h"
#include "MainMenuView.h"
#include "HowToPlayView.h"
#include "Observer.h"
#include "MatchView.h"
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

} GameController;

static int private_gameGoing(GameController* self);

void GameController_endMatch(GameController* self)
{
    Board_destroy(self->board);
    self->board = NULL;
    MatchView_destroy(self->currentMatchView);
    self->currentMatchView = NULL;
}



GameController* GameController_new()
{
    GameController* created = (GameController*)malloc(sizeof(GameController));
    created->observingBoard = NULL;
    created->currentMainMenuView = NULL;
    created->currentMatchView = NULL;
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
void GameController_jump(GameController* self, Vector2D from, Vector2D to)
{
    if (Board_tryJump(self->board, from, to))
    {
        Score_increment(self->score);
    }    
    else
    {
        printf("Error: invalid jump data.\n");
    }
}

void GameController_activate(GameController* self, Vector2D at)
{
    if (Board_tryActivate(self->board, at))
    {

    }
    else 
    {
        printf("Error: invalid activation data.\n");
    }
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

    if (private_gameGoing(self))
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

static void private_recieveSignal(void* vSelf, const char* signalID, void* signalArgs);

void private_recieveSignal(void* vSelf, const char* signalID, void* signalArgs)
{
    if (strncmp(signalID, "jump", strlen(signalID)) == 0)
    {
        Score_increment(((GameController*)vSelf)->score);
    }
}

void GameController_restartGame(GameController* self)
{
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
    Board_clickField(self->board, coords);

}

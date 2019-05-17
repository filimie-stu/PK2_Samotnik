#include "ScoreViewModel.h"
#include "GameController.h"
#include "MainMenuView.h"
#include "HowToPlayView.h"
#include "Observer.h"
#include "MatchView.h"
#include "IScore.h"
#include "JumpHistory.h"
#include "IModelFactory.h"
#include "BoardViewModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct game_controller
{
    JumpHistory* jumpHistory;
    IModelFactory* modelFactory;
    IBoard* board;
    IScore* score;
    Observer* observingIBoard;
    MatchView* currentMatchView;
    MainMenuView* currentMainMenuView;

} GameController;

static int private_gameGoing(GameController* self);

BoardViewModel private_boardToViewModel(IBoard* board)
{
    BoardViewModel viewModel;
    viewModel.boardObservable = IBoard_asObservable(board);
    viewModel.dimensions = IBoard_getDimensions(board);
    for (int i = 0; i < viewModel.dimensions.x; i++)
    {
        for (int j = 0; j < viewModel.dimensions.y; j++)
        {
            Vector2D coords = {i,j};
            viewModel.fields[i][j] = IBoard_getFieldAt(board, coords);
        }
    }
    return viewModel;    
}

ScoreViewModel private_scoreToViewModel(IScore *score)
{
    ScoreViewModel viewModel;
    viewModel.goal = IScore_getGoal(score);
    viewModel.points = IScore_getPoints(score);
    viewModel.scoreObservable = IScore_asObservable(score);
    return viewModel;
}

void GameController_endMatch(GameController* self)
{
    IBoard_destroy(self->board, 1);
    self->board = NULL;
    MatchView_destroy(self->currentMatchView);
    self->currentMatchView = NULL;
}



GameController* GameController_new(IModelFactory* modelFactory)
{
    GameController* created = (GameController*)malloc(sizeof(GameController));
    created->modelFactory=modelFactory;

    created->observingIBoard = NULL;
    created->currentMainMenuView = NULL;
    created->currentMatchView = NULL;
    created->board = NULL;
    created->score = NULL;
    created->jumpHistory = NULL;
    
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
    if (IBoard_tryJump(self->board, from, to))
    {
        IScore_increment(self->score);
    }    
    else
    {
        printf("Error: invalid jump data.\n");
    }
}

void GameController_activate(GameController* self, Vector2D at)
{
    if (IBoard_tryActivate(self->board, at))
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
    // if (strncmp(signalID, "jump", strlen(signalID)) == 0)
    // {
    //     IScore_increment(((GameController*)vSelf)->score);
    // }
}

void GameController_restartGame(GameController* self)
{
    IBoard_destroy(self->board,1);
    Observer_dispose(self->observingIBoard);
    self->board = IModelFactory_createBoard(self->modelFactory);
    self->observingIBoard = Observer_new(self, private_recieveSignal, IBoard_asObservable(self->board));

    IScore_destroy(self->score);
    self->score = IModelFactory_createScore(self->modelFactory);

    MatchView_destroy(self->currentMatchView);
    self->currentMatchView = MatchView_new(self, private_boardToViewModel(self->board), private_scoreToViewModel(self->score));    
    
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
        IBoard_destroy(self->board, 1);
        IScore_destroy(self->score);
        Observer_dispose(self->observingIBoard);
    }

    self->board = IModelFactory_createBoard(self->modelFactory);
    self->score = IModelFactory_createScore(self->modelFactory);
    self->jumpHistory = JumpHistory_new(IBoard_asObservable(self->board));
    self->observingIBoard = Observer_new(self, private_recieveSignal, IBoard_asObservable(self->board));
    self->currentMatchView = MatchView_new(self, private_boardToViewModel(self->board), private_scoreToViewModel(self->score));
    MatchView_display(self->currentMatchView);
}
void GameController_rollback(GameController* self)
{
    IBoard_rollbackJump(self->board, JumpHistory_extract(self->jumpHistory)); 
    IScore_decrement(self->score);
}

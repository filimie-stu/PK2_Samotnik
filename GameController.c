#include "NewGameArgs.h"
#include "ScoreViewModel.h"
#include "GameController.h"
#include "HowToPlayDialog.h"
#include "Observer.h"
#include "IScore.h"
#include "IJumpHistory.h"
#include "IModelFactory.h"
#include "BoardViewModel.h"
#include "IViewFactory.h"
#include "IGameController.h"
#include "GameControllerTypeWrappers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct game_controller
{
    IGameController *iGameController;
    IModelFactory *modelFactory;
    IViewFactory *viewFactory;
    IBoard *board;
    IScore *score;
    IJumpHistory *jumpHistory;
    // Observer* observingIBoard;

    IView *matchView;
    IView *mainMenuView;
    IView *gameOverView;
    NewGameArgs lastUsedSettings;

} GameController;

static int private_gameGoing(GameController *self);
static BoardViewModel private_boardToViewModel(IBoard *board);
static ScoreViewModel private_scoreToViewModel(IScore *score);
static GameOverViewModel private_getWonViewModel();
static GameOverViewModel private_getLostViewModel();

GameController *GameController_new(IModelFactory *modelFactory, IViewFactory *viewFactory)
{
    GameController *created = (GameController *)malloc(sizeof(GameController));
    created->iGameController = IGameController_new(
        created,
        private_wrapper_destroy,
        private_wrapper_mainMenu,
        private_wrapper_prepareForExit,
        private_wrapper_beginMatch,
        private_wrapper_restartGame,
        private_wrapper_continueMatch,
        private_wrapper_endMatch,
        private_wrapper_rollback,
        private_wrapper_jump,
        private_wrapper_activate);
    created->modelFactory = modelFactory;
    created->viewFactory = viewFactory;
    // created->observingIBoard = NULL;
    created->mainMenuView = NULL;
    created->matchView = NULL;
    created->gameOverView = NULL;
    created->board = NULL;
    created->score = NULL;
    created->jumpHistory = NULL;

    return created;
}

void GameController_endMatch(GameController *self)
{
    IView_destroy(self->gameOverView, 1);
    self->gameOverView = NULL;
    IBoard_destroy(self->board, 1);
    self->board = NULL;
    IView_destroy(self->matchView, 1);
    self->matchView = NULL;
    GameController_mainMenu(self);
}

void GameController_beginMatch(GameController *self, NewGameArgs settings)
{
    self->lastUsedSettings = settings;
    if (self->mainMenuView)
    {
        IView_hide(self->mainMenuView);
    }
    if (private_gameGoing(self))
    {
        IBoard_destroy(self->board, 1);
        IScore_destroy(self->score, 1);
    }
    self->board = IModelFactory_createBoard(self->modelFactory, settings.boardFilename);
    self->score = IModelFactory_createScore(self->modelFactory, IBoard_countTokens(self->board), settings.handicap);
    self->jumpHistory = IModelFactory_createJumpHistory(self->modelFactory);

    MatchViewModel viewModel = {private_boardToViewModel(self->board), private_scoreToViewModel(self->score)};
    self->matchView = IViewFactory_createMatchView(
        self->viewFactory,
        GameController_asIGameController(self),
        viewModel);
    IView_display(self->matchView);
}
void GameController_rollback(GameController *self)
{
    if (!IJumpHistory_isEmpty(self->jumpHistory))
    {
        IBoard_rollbackJump(self->board, IJumpHistory_extract(self->jumpHistory));
        IScore_decrement(self->score);
    }
}

void GameController_prepareForExit(GameController *self)
{
    //...
}
void GameController_restartGame(GameController *self)
{
    if (self->gameOverView)
    {
        IView_destroy(self->gameOverView, 1);
        self->gameOverView = NULL;
    }

    IBoard_destroy(self->board, 1);
    self->board = IModelFactory_createBoard(self->modelFactory, self->lastUsedSettings.boardFilename);

    IScore_destroy(self->score, 1);
    self->score = IModelFactory_createScore(self->modelFactory, IBoard_countTokens(self->board), self->lastUsedSettings.handicap);

    IJumpHistory_destroy(self->jumpHistory, 1);
    self->jumpHistory = IModelFactory_createJumpHistory(self->modelFactory);

    IView_destroy(self->matchView, 1);
    MatchViewModel viewModel = {private_boardToViewModel(self->board),
                                private_scoreToViewModel(self->score)};

    self->matchView = IViewFactory_createMatchView(
        self->viewFactory,
        GameController_asIGameController(self),
        viewModel);

    IView_display(self->matchView);
}

void GameController_continueMatch(GameController *self)
{
    if (!self->matchView)
    {
        perror("Error: no ongoing match to continue.\n");
        return;
    }

    IView_destroy(self->mainMenuView, 1);
    IView_display(self->matchView);
}
void GameController_jump(GameController *self, Vector2D from, Vector2D to)
{
    JumpInfo jumpData;
    if (IBoard_tryJump(self->board, from, to, &jumpData))
    {
        IScore_increment(self->score);
        IJumpHistory_addRecord(self->jumpHistory, jumpData);

        if (IScore_hasWon(self->score))
        {
            self->gameOverView = IViewFactory_createGameOverView(
                self->viewFactory,
                GameController_asIGameController(self),
                private_getWonViewModel());
            IView_display(self->gameOverView);
        }
        else if (IBoard_isDeadEnd(self->board))
        {
            self->gameOverView = IViewFactory_createGameOverView(
                self->viewFactory,
                GameController_asIGameController(self),
                private_getLostViewModel());
            IView_display(self->gameOverView);
        }
    }
    else
    {
        printf("Error: invalid jump data.\n");
    }
}

void GameController_activate(GameController *self, Vector2D at)
{
    if (IBoard_tryActivate(self->board, at))
    {
    }
    else
    {
        printf("Error: invalid activation data.\n");
    }
}

void GameController_destroy(GameController *self)
{
    if (self->matchView)
    {
        IView_destroy(self->matchView, 1);
    }
    if (self->mainMenuView)
    {
        IView_destroy(self->mainMenuView, 1);
    }
    IGameController_destroy(self->iGameController, 0);
    free(self);
}

void GameController_mainMenu(GameController *self)
{
    if (private_gameGoing(self))
    {
        IView_hide(self->matchView);
    }
    MainMenuViewModel viewModel = {private_gameGoing(self)};
    self->mainMenuView = IViewFactory_createMainMenuView(
        self->viewFactory,
        GameController_asIGameController(self),
        viewModel);
    IView_display(self->mainMenuView);
}
IGameController *GameController_asIGameController(GameController *self)
{
    return self->iGameController;
}
int private_gameGoing(GameController *self)
{
    if (self->board)
        return 1;

    return 0;
}

BoardViewModel private_boardToViewModel(IBoard *board)
{
    BoardViewModel viewModel;
    viewModel.boardObservable = IBoard_asObservable(board);
    viewModel.dimensions = IBoard_getDimensions(board);
    for (int i = 0; i < viewModel.dimensions.x; i++)
    {
        for (int j = 0; j < viewModel.dimensions.y; j++)
        {
            Vector2D coords = {i, j};
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

GameOverViewModel private_getWonViewModel()
{
    GameOverViewModel vm = {"You win!", "Congratulations, you've reached your goal!"};
    return vm;
}

GameOverViewModel private_getLostViewModel()
{
    GameOverViewModel vm = {"You lose!", "There are no more valid moves!"};
    return vm;
}
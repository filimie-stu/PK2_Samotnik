#include "RedirectArgs.h"
#include "IView.h"
#include "IBoard.h"
#include "IScore.h"
#include "IJumpHistory.h"
#include "NewGameArgs.h"
#include "ScoreViewModel.h"
#include "BoardViewModel.h"
#include "IModelFactory.h"
#include "Observable.h"
#include <stdlib.h>
#include <stdio.h>

// typedef struct view_factory IViewFactory;
// IView* IViewFactory_createMatchView(IViewFactory* self, BoardViewModel bvm, ScoreViewModel svm);


// typedef struct match_controller
// {
//     Observable* observableByRouter;
//     IModelFactory *modelFactory;
//     IViewFactory *viewFactory;
//     IView* matchView;
//     IBoard* board;
//     IScore* score;
//     IJumpHistory* jumpHistory;
// } MatchController;
// static BoardViewModel private_boardToViewModel(IBoard* board);
// static ScoreViewModel private_scoreToViewModel(IScore* score);

// MatchController* MatchController_new(IModelFactory* modelFactory, IViewFactory* viewFactory)
// {
//     MatchController* created = (MatchController*)malloc(sizeof(MatchController));
//     created->observableByRouter = Observable_new(created);
//     created->modelFactory = modelFactory;
//     created->viewFactory = viewFactory;
//     created->matchView = NULL;
//     created->board = NULL;
//     created->score = NULL;
//     created->jumpHistory = NULL;

//     return created;
// }

// void MatchController_destroy(MatchController* self)
// {
//     Observable_dispose(self->observableByRouter); // needs additional param
//     IView_destroy(self->matchView, 1);
//     IBoard_destroy(self->board, 1);
//     IScore_destroy(self->score); // needs additional param
//     IJumpHistory_destroy(self->jumpHistory, 1);

//     free(self);
// }   

// void MatchController_beginMatch(MatchController* self, NewGameArgs settings)
// {
//     IView_destroy(self->matchView, 1);
//     IBoard_destroy(self->board, 1);
//     IScore_destroy(self->score);
//     IJumpHistory_destroy(self->jumpHistory, 1);

//     self->board = IModelFactory_createBoard(self->modelFactory, settings.boardFilename);
//     self->score = IModelFactory_createScore(self->modelFactory, IBoard_countTokens(self->board), settings.handicap);
//     self->jumpHistory = IModelFactory_createJumpHistory(self->modelFactory);

//     BoardViewModel bvm = private_boardToViewModel(self->board);
//     ScoreViewModel svm = private_scoreToViewModel(self->score);

//     self->matchView = ViewFactory_createMatchView(self->viewFactory, bvm, svm);
//     IView_display(self->matchView);
// }

// void MatchController_resumeMatch(MatchController* self)
// {
//     if (!self->matchView)
//     {
//         printf("Error: there's no match to resume.\n");
//         return;
//     }
//     IView_display(self->matchView);
// }

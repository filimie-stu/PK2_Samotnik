#pragma once
#include "BoardViewModel.h"
#include "ScoreViewModel.h"
#include "IGameController.h"

typedef struct match_view_model 
{
    BoardViewModel boardVM;
    ScoreViewModel scoreVM;

} MatchViewModel;
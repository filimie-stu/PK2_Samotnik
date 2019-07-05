#pragma once
#include "BoardViewModel.h"
#include "ScoreViewModel.h"
#include "IGameController.h"

/**
 * \brief Model głównego widoku gry.
 * 
 * Jest to prosta struktura zawierająca zewnętrzne informacje potrzebne do poprawnego wyświetlenia widoku.
*/
typedef struct match_view_model 
{
    BoardViewModel boardVM; //!< boardVM model widoku planszy.
    ScoreViewModel scoreVM; //!< scoreVM model widoku wyniku.

} MatchViewModel;
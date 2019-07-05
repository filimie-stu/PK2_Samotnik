#pragma once

/**
 * \brief Struktura przechowująca treści komunikatów wyświetlanych po zakończeniu gry
*/
typedef struct game_over_view_model
{
    const char* primaryText;            //!< treść nagłówka.
    const char* detailedDescription;    //!< zasadnicza treść komunikatu.

} GameOverViewModel;
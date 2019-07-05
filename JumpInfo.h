#pragma once
#include "Vector2D.h"


/**
 * \brief Struktura grupująca dane dotyczące skoku pionkiem.
*/
typedef struct jump_info
{
    Vector2D from;      //!< współrzędne początkowej pozycji pionka wykonującego ruch.
    Vector2D through;   //!< współrzędne pionka zbitego.
    Vector2D to;        //!< współrzędne końcowej pozycji pionka wykonującego ruch.
       
} JumpInfo;
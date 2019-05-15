#pragma once
#include "Vector2D.h"
typedef struct activate_args
{
    Vector2D previouslyActive;
    Vector2D activated;
    Vector2D jumpSpots[4];

} ActivateArgs;
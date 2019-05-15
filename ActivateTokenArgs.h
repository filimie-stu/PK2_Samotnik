#pragma once
#include "Vector2D.h"


typedef struct activate_token_args
{
    Vector2D activeCoords;
    Vector2D jumpSpots[4];
} ActivateTokenArgs;
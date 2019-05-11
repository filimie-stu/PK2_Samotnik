#include "Vector2D.h"
#include <math.h>


Vector2D Vector2D_calculateMidpoint(Vector2D first, Vector2D second)
{
    Vector2D midPoint;
    midPoint.x = (first.x + second.x)/2;
    midPoint.y = (first.y + second.y)/2;
    return midPoint;
}
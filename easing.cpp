#include "easing.h"

float easeInOutCubic(float x)
{
    if (x < 0.5f)
    {
        return 4.0f * x * x * x;
    }
    else
    {
        float t = -2.0f * x + 2.0f;
        return 1.0f - (t * t * t) * 0.5f;
    }
}

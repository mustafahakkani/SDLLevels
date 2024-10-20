#include "Timing.h"

Timing::Timing()
{
    fpsCount = 0;
    fpsLast = 0;
    deltaTime = 0;
    currentTime = SDL_GetTicks();
    lastTime = currentTime;
    fpsStart = currentTime;
}

void Timing::Tick()
{
   currentTime = SDL_GetTicks();
    deltaTime = (float)(currentTime - lastTime) / 1000.0f;

    if (fpsStart + 1000 <= currentTime)
    {
        fpsLast = fpsCount;
        fpsCount = 0;
        fpsStart = currentTime;
    }
    else
    {
        fpsCount++;
    }

    lastTime = currentTime;
    lastTime = currentTime;
}

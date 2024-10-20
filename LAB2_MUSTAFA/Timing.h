#ifndef TIMING_H
#define TIMING_H

#include "StandardIncludes.h"

class Timing : public Singleton<Timing>
{
public:
    // Constructors/Destructors
    Timing();
    virtual ~Timing() {}

    // Accessors
    unsigned int GetFPS() { return fpsLast; }
    float GetDeltaTime() { return deltaTime; }

    // Methods
    void Tick();

private:
    // Members
    unsigned int currentTime;
    unsigned int lastTime;
    unsigned int fpsStart;
    unsigned int fpsCount;
    unsigned int fpsLast;
    float deltaTime;
};

#endif // TIMING_H

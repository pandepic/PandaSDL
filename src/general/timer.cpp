#include "timer.h"

PandaSDL::Timer::Timer()
{
}

PandaSDL::Timer::~Timer()
{
}

void PandaSDL::Timer::Start()
{
    _prevTicks = SDL_GetTicks();
}

void PandaSDL::Timer::Tick()
{
    _prevTicks = SDL_GetTicks();
}

unsigned int PandaSDL::Timer::GetTicks() const
{
    return SDL_GetTicks() - _prevTicks;
}

double PandaSDL::Timer::GetDelta() const
{
    double ticks = GetTicks();
    return ticks / 1000.0;
}

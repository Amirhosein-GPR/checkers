#include "core/Timer.hpp"

Timer::Timer()
{
    this->startTicks = 0;
    this->pausedTicks = 0;
    this->started = false;
    this->paused = false;
}

void Timer::start()
{
    this->started = true;
    this->paused = false;
    this->startTicks = SDL_GetTicks();
    this->pausedTicks = 0;
}

void Timer::stop()
{
    this->started = false;
    this->paused = false;
    this->startTicks = 0;
    this->pausedTicks = 0;
}

void Timer::pause()
{
    if (this->started && !this->paused)
    {
        this->paused = true;
        this->pausedTicks = SDL_GetTicks() - this->startTicks;
        this->startTicks = 0;
    }
}

void Timer::resume()
{
    if (this->started && this->paused)
    {
        this->paused = false;
        this->startTicks = SDL_GetTicks() - this->pausedTicks;
        this->pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    if (this->started)
    {
        if (this->paused)
        {
            return this->pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - this->startTicks;
        }
    }

    return 0;
}

bool Timer::isStarted()
{
    return this->started;
}

bool Timer::isPaused()
{
    return this->paused && this->started;
}
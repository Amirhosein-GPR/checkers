#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL2/SDL.h>

class Timer
{
    public:
        Timer();
        void start();
        void stop();
        void pause();
        void resume();

        Uint32 getTicks();

        bool isStarted();
        bool isPaused();
    private:
        Uint32 startTicks;
        Uint32 pausedTicks;
        bool started;
        bool paused;
};

#endif
#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>
#include "core/Timer.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const float totalTicksForEachFrame = 1000.f / SCREEN_FPS;

enum ErrorType
{
    ERROR_TYPE_SDL,
    ERROR_TYPE_TTF,
    ERROR_TYPE_IMG,
    ERROR_TYPE_MIX,
    ERROR_TYPE_NO_ERROR
};

class System
{
    public:
        static void logError(std::string const &message, ErrorType errorType = ERROR_TYPE_NO_ERROR);
};

#endif
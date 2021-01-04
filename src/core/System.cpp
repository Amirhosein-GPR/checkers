#include "core/System.hpp"

void System::logError(std::string const &message, ErrorType errorType)
{
    std::string tempMessage = "Error " + message + ' ';

    if (errorType != ERROR_TYPE_NO_ERROR)
    {
        switch (errorType)
        {
            case ERROR_TYPE_SDL:
                tempMessage += SDL_GetError();
            break;
            case ERROR_TYPE_TTF:
                tempMessage += TTF_GetError();
            break;
            case ERROR_TYPE_IMG:
                tempMessage += IMG_GetError();
            break;
            case ERROR_TYPE_MIX:
                tempMessage += Mix_GetError();
            break;
        }
    }
    
    std::cerr << tempMessage;
}
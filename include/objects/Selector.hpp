#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include "core/Object.hpp"

class Selector : public Object
{
    public:
        Selector(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
        ~Selector();
    private:
        Uint8 blockPosition;
};

#endif
#ifndef REDPIECE_HPP
#define REDPIECE_HPP

#include "core/Object.hpp"

class RedPiece : public Object
{
    public:
        RedPiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
};

#endif
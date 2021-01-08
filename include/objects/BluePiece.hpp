#ifndef BLUEPIECE_HPP
#define BLUEPIECE_HPP

#include "core/Object.hpp"

class BluePiece : public Object
{
    public:
        BluePiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
};

#endif
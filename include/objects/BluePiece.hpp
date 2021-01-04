#ifndef BLUEPIECE_HPP
#define BLUEPIECE_HPP

#include "core/Object.hpp"

class BluePiece : public Object
{
    public:
        BluePiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
        void render(int delta, SDL_Rect *clip = NULL, int width = 0, int height = 0) override;
};

#endif
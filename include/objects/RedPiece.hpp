#ifndef REDPIECE_HPP
#define REDPIECE_HPP

#include "core/Object.hpp"

class RedPiece : public Object
{
    public:
        RedPiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
        void render(int delta, SDL_Rect *clip = NULL, int width = 0, int height = 0) override;
};

#endif
#ifndef BLUEPIECE_HPP
#define BLUEPIECE_HPP

#include "objects/Piece.hpp"

class BluePiece : public Piece
{
    public:
        BluePiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
};

#endif
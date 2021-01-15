#ifndef REDPIECE_HPP
#define REDPIECE_HPP

#include "objects/Piece.hpp"

class RedPiece : public Piece
{
    public:
        RedPiece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
};

#endif
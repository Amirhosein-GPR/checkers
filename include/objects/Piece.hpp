#ifndef PIECE_HPP
#define PIECE_HPP

#include "core/Object.hpp"

class Piece : public Object
{
    public:
        Piece(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
};

#endif
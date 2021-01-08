#include "objects/BluePiece.hpp"

BluePiece::BluePiece(Vector position, SDL_Renderer *renderer, TTF_Font *font) : Object(position, renderer, font)
{
    SDL_Rect *BluePieceClip = new SDL_Rect{32, 0, 32, 32};
    this->clip = BluePieceClip;
}
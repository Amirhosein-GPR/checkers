#include "objects/RedPiece.hpp"

RedPiece::RedPiece(Vector position, SDL_Renderer *renderer, TTF_Font *font) : Object(position, renderer, font)
{
    SDL_Rect *RedPieceClip = new SDL_Rect{0, 0, 32, 32};
    this->clip = RedPieceClip;
}
#include "objects/RedPiece.hpp"

RedPiece::RedPiece(Vector position, SDL_Renderer *renderer, TTF_Font *font) : Object(position, renderer, font)
{

}

void RedPiece::render(int delta, SDL_Rect *clip, int width, int height)
{
    if (clip == NULL)
    {
        SDL_Rect RedPieceClip = {0, 0, 32, 32};
        Object::render(delta, &RedPieceClip, width, height);
    }
}
#include "objects/BluePiece.hpp"

BluePiece::BluePiece(Vector position, SDL_Renderer *renderer, TTF_Font *font) : Object(position, renderer, font)
{

}

void BluePiece::render(int delta, SDL_Rect *clip, int width, int height)
{
    if (clip == NULL)
    {
        SDL_Rect BluePieceClip = {32, 0, 32, 32};
        Object::render(delta, &BluePieceClip, width, height);
    }
}
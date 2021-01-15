#include "objects/Selector.hpp"

Selector::Selector(Vector position, SDL_Renderer *renderer, TTF_Font *font) : Object(position, renderer, font)
{
    this->blockPosition = 0;
}

Selector::~Selector()
{

}

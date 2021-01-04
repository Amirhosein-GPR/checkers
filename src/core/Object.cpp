#include "core/Object.hpp"

SDL_Renderer *Object::renderer = nullptr;
TTF_Font *Object::font = nullptr;

Object::Object()
{

}

Object::Object(Vector position, SDL_Renderer *renderer, TTF_Font *font)
{
    if (Object::renderer == nullptr && Object::font == nullptr && renderer != nullptr && font != nullptr)
    {
        Object::renderer = renderer;
        Object::font = font;
    }
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->position.x = position.x;
    this->position.y = position.y;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->acceleration.x = 0;
    this->acceleration.y = 0;
}

Object::~Object()
{
    Object::free(this);
}

bool Object::loadFromFile(const std::string &path)
{
    Object::free(this);
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        System::logError("Unable to load image " + path, ERROR_TYPE_IMG);
        return false;
    }
    else
    {
        this->texture = SDL_CreateTextureFromSurface(Object::renderer, surface);
        if (this->texture == NULL)
        {
            System::logError("Unable to create texture from " + path, ERROR_TYPE_SDL);
            return false;
        }
        else
        {
            this->width = surface->w;
            this->height = surface->h;
        }

        SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(surface);
    }
    return true;
}

bool Object::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    Object::free(this);
    SDL_Surface *surface = TTF_RenderText_Solid(Object::font, textureText.c_str(), textColor);
    if (surface == NULL)
    {
        System::logError("Unable to render text surface!", ERROR_TYPE_TTF);
        return false;
    }
    else
    {
        this->texture = SDL_CreateTextureFromSurface(Object::renderer, surface);
        if (this->texture == NULL)
        {
            System::logError("Unable to create texture from rendered text!", ERROR_TYPE_SDL);
            SDL_FreeSurface(surface);
            return false;
        }
        else
        {
            this->width = surface->w;
            this->height = surface->h;
        }

        SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(surface);
        return true;
    }
}

void Object::loadFromOtherObject(Object *object)
{
    this->width = object->width;
    this->height = object->height;
    this->texture = object->texture;
}

void Object::setVelocity(Vector velocity)
{
    this->velocity.x = velocity.x;
    this->velocity.y = velocity.y;
}

void Object::setAcceleration(Vector acceleration)
{

}

void Object::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(this->texture, alpha);
}

void Object::updatePhysics(Object *object)
{
    if (object->velocity.x < 0)
    {
        object->acceleration.x = 0;
        object->velocity.x = 0;
    }
    if (object->velocity.y < 0)
    {
        object->acceleration.y = 0;
        object->velocity.y = 0;
    }
    if ((int) object->position.x == (int) object->destination.x && (int) object->position.y == (int) object->destination.y)
    {
        object->velocity.x = 0;
        object->velocity.y = 0;
    }
}

void Object::free(Object *object)
{
    if (object->texture != nullptr)
    {
        SDL_DestroyTexture(object->texture);
        object->texture = nullptr;
        object->width = 0;
        object->height = 0;
        object->velocity.x = 0;
        object->velocity.y = 0;
        object->acceleration.x = 0;
        object->acceleration.y = 0;
    }
}

void Object::render(int delta, SDL_Rect *clip, int width, int height)
{
    SDL_Rect textureQuad;
    if (delta == 0)
    {
        delta = 1;
    }

    this->position.x += (this->velocity.x) * delta / SCREEN_FPS;
    this->position.y += (this->velocity.y) * delta / SCREEN_FPS;

    if (clip == NULL)
    {
        textureQuad = {(int) this->position.x, (int) this->position.y, this->width, this->height};
    }
    else
    {
        textureQuad = {(int) this->position.x, (int) this->position.y, clip->w, clip->h};
    }

    if (width != 0 && height != 0)
    {
        textureQuad = {(int) this->position.x, (int) this->position.y, width, height};
    }

    SDL_RenderCopy(this->renderer, this->texture, clip, &textureQuad);
}

void Object::animate(Vector position, int duration)
{
    if (this->position.x != position.x)
    {
        this->velocity.x = (position.x - this->position.x) / duration;
    }

    if (this->position.y != position.y)
    {
        this->velocity.y = (position.y - this->position.y) / duration;
    }

    this->destination.x = position.x;
    this->destination.y = position.y;

    // this->acceleration.x = 0.1;
    // this->acceleration.y = 0.1;
}

int Object::getWidth()
{
    return this->width;
}

int Object::getHeight()
{
    return this->height;
}

Vector Object::getVelocity()
{
    return this->velocity;
}
Vector Object::getAcceleration()
{
    return this->acceleration;
}
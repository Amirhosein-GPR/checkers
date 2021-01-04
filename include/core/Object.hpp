#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "System.hpp"

struct Vector
{
    public:
        float x;
        float y;
        Vector(int x, int y)
        {
            this->x = x;
            this->y = y;
        }
        Vector()
        {
            this->x = 0;
            this->y = 0;
        }
};

class Object
{
    public:
        Object(Vector position, SDL_Renderer *renderer = nullptr, TTF_Font *font = nullptr);
        ~Object();
        bool loadFromFile(const std::string &path);
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
        void loadFromOtherObject(Object *object);
        void setVelocity(Vector velocity);
        void setAcceleration(Vector acceleration);
        void setAlpha(Uint8 alpha);
        virtual void render(int delta, SDL_Rect *clip = NULL, int width = 0, int height = 0);
        void animate(Vector position, int duration);
        int getWidth();
        int getHeight();
        Vector getVelocity();
        Vector getAcceleration();
        static void updatePhysics(Object *object);
    private:
        Object();
        static void free(Object *object);
        static SDL_Renderer *renderer;
        static TTF_Font *font;
        SDL_Texture *texture;
        Vector position;
        Vector destination;
        int width;
        int height;
        Vector velocity;
        Vector acceleration;
};

#endif
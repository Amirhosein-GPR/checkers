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
        bool loadFromRenderedText(const std::string &textureText, SDL_Color textColor);
        void loadFromOtherObject(Object *object);
        void setClip(SDL_Rect *clip);
        void setVelocity(Vector velocity);
        void setAcceleration(Vector acceleration);
        void setPosition(float x, float y);
        void setAlpha(Uint8 alpha);
        void setBlockPosition(Uint8 blockPostion);
        Vector getPositionEquvalant(Uint8 inputBlockPostion);
        Uint8 getBlockPosition();
        void render(int delta);
        void animate(Vector position, int duration);
        void animateBlockPosition(int blockPosition, int duration);
        void animate(Uint8 alpha, int duration, bool reverseLoop = false);
        int getWidth();
        int getHeight();
        Vector getVelocity();
        Vector getAcceleration();
        static void updatePhysics(Object *object);

    protected:
        SDL_Rect *clip;

    private:
        Object();
        static void free(Object *object);
        static SDL_Renderer *renderer;
        static TTF_Font *font;
        SDL_Texture *texture;
        Vector position;
        Vector destination;
        Uint8 destinationAlpha;
        int width;
        int height;
        Vector velocity;
        Vector acceleration;
        Uint8 alpha;
        float alphaPerSecond;
        bool reverseLoop;
        Uint8 blockPosition;
};

#endif
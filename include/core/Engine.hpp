#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/System.hpp"
#include "core/Object.hpp"
#include "objects/RedPiece.hpp"
#include "objects/BluePiece.hpp"

class Engine
{
    public:
        static Engine * getInstance();
        bool initialize();
        bool loadMedia();
        
        void update();
        void render();
        bool shouldClose();
        static Timer *timer;
    private:
        Engine();
        ~Engine();
        void loadAndShowIntro();
        void loadAndShowNameInput();
        void loadAndShowMainObjects();
        static Engine *instance;
        static SDL_Window *window;
        static SDL_Renderer *renderer;
        static SDL_Event *event;
        static std::vector<Object *> objects;
        static TTF_Font *font;
        static std::vector<Object *>::iterator objectsIterator;
        static bool close;
};

#endif
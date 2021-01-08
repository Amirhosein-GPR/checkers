#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/System.hpp"
#include "core/Object.hpp"
#include "objects/RedPiece.hpp"
#include "objects/BluePiece.hpp"

enum GameStateMachine
{
    GAME_STATE_INTRO,
    GAME_STATE_INPUT,
    GAME_STATE_GAME,
    GAME_STATE_OUTRO,
    GAME_STATE_EXIT
};

class Engine
{
    public:
        static Engine * getInstance();
        bool initialize();
        void runThenGo(void (*function)(), Uint32 goToNextInThisTime, GameStateMachine gameState, bool lilimtedTime = true);
        static void loadIntro();
        static void loadNameInput();
        static void loadMainGame();
        static void loadOutro();
        static void exitTheGame();
        void manageStates();
        void deleteObjectsTillNow();
        void update();
        void render();
        Timer *frameTimer;
        Timer *mainTimer;
        GameStateMachine gameState;
        bool firstMediaLoad;
        static bool running;
    private:
        Engine();
        ~Engine();
        static Engine *instance;
        static SDL_Window *window;
        static SDL_Renderer *renderer;
        static SDL_Event *event;
        static std::vector<Object *> objects;
        static TTF_Font *font;
        static std::vector<Object *>::iterator objectsIterator;
        static std::string username;
        static bool usernameSholudUpdate;
        static int usernameIndex;
        static void appendInputText(const char &character);
        static void backspaceInputText();
};

#endif
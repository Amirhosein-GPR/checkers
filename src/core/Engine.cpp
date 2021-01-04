#include "core/Engine.hpp"

Engine * Engine::instance = nullptr;
SDL_Window * Engine::window = nullptr;
SDL_Renderer * Engine::renderer = nullptr;
SDL_Event * Engine::event = nullptr;
TTF_Font * Engine::font = nullptr;
Timer * Engine::timer = nullptr;

std::vector<Object *> Engine::objects;
std::vector<Object *>::iterator Engine::objectsIterator;

bool Engine::close = false;

Engine::Engine()
{}

Engine::~Engine()
{
    SDL_DestroyRenderer(Engine::renderer);
    SDL_DestroyWindow(Engine::window);

    Engine::renderer = nullptr;
    Engine::window = nullptr;

    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        delete *Engine::objectsIterator;
    }

    delete Engine::event;
    delete Engine::timer;
    delete Engine::instance;

    TTF_CloseFont(Engine::font);

    delete Engine::font;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

Engine * Engine::getInstance()
{
    return (Engine::instance == nullptr) ? Engine::instance = new Engine() : Engine::instance;
}

bool Engine::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        System::logError("SDL could not initialize!", ERROR_TYPE_SDL);
        return false;
    }
    else
    {
        Engine::window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Engine::window == NULL)
        {
            System::logError("Window could not be created!", ERROR_TYPE_SDL);
            return false;
        }
        else
        {
            Engine::renderer = SDL_CreateRenderer(Engine::window, -1, SDL_RENDERER_ACCELERATED);

            if (Engine::renderer == NULL)
            {
                System::logError("Renderer could not be created!", ERROR_TYPE_SDL);
                return false;
            }
            else
            {
                if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG))
                {
                    System::logError("SDL_image could not initialize!", ERROR_TYPE_IMG);
                    return false;
                }
                else
                {
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                    {
                        System::logError("SDL_mixer could not initialize!", ERROR_TYPE_MIX);
                        return false;
                    }
                    else
                    {
                        if (TTF_Init() == -1)
                        {
                            System::logError("SDL_ttf could not initialize!", ERROR_TYPE_TTF);
                            return false;
                        }
                        else
                        {
                            Engine::font = TTF_OpenFont("assets/font/DejaVuSansCondensed.ttf", 20);
                            if (Engine::font == NULL)
                            {
                                System::logError("Failed to load DejaVuSansCondensed font!", ERROR_TYPE_TTF);
                                return false;
                            }
                            else
                            {
                                SDL_SetRenderDrawColor(Engine::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                            }
                        }
                    }
                }
            }
        }
    }
    Engine::event = new SDL_Event();
    Engine::timer = new Timer();
    return true;
}

bool Engine::loadMedia()
{
    Object *infoBar = new Object(Vector(0, 0), Engine::renderer, Engine::font);
    infoBar->loadFromFile("assets/image/Info.png");
    Engine::objects.push_back(infoBar);

    Object *board = new Object(Vector(560, 0));
    board->loadFromFile("assets/image/Board.png");
    Engine::objects.push_back(board);

    Object *redPiece1 = new RedPiece(Vector(652, 90));
    redPiece1->loadFromFile("assets/image/Pieces.png");
    Engine::objects.push_back(redPiece1);

    int x = 724;
    for (int i = 0; i < 7; i++)
    {
        RedPiece *redPiece2To8 = new RedPiece(Vector(x, 90));
        redPiece2To8->loadFromOtherObject(redPiece1);
        Engine::objects.push_back(redPiece2To8);
        x += 72;
    }
    x = 652;
    for (int i = 0; i < 8; i++)
    {
        RedPiece *redPiece9To16 = new RedPiece(Vector(x, 162));
        redPiece9To16->loadFromOtherObject(redPiece1);
        Engine::objects.push_back(redPiece9To16);
        x += 72;
    }
    x = 652;
    for (int i = 0; i < 8; i++)
    {
        BluePiece *bluePiece1To8 = new BluePiece(Vector(x, 524));
        bluePiece1To8->loadFromOtherObject(redPiece1);
        Engine::objects.push_back(bluePiece1To8);
        x += 72;
    }
    x = 652;
    for (int i = 0; i < 8; i++)
    {
        BluePiece *bluePiece9To16 = new BluePiece(Vector(x, 597));
        bluePiece9To16->loadFromOtherObject(redPiece1);
        Engine::objects.push_back(bluePiece9To16);
        x += 72;
    }


    Object *text = new Object(Vector(256, 0));
    text->loadFromRenderedText("This is a text yohoooooo", SDL_Color {0, 128, 64});
    Engine::objects.push_back(text);
    return true;
}

void Engine::loadAndShowIntro()
{
    
}

void Engine::loadAndShowNameInput()
{}

void Engine::loadAndShowMainObjects()
{}

void Engine::update()
{
    while (SDL_PollEvent(Engine::event) != 0)
    {
        if (Engine::event->type == SDL_QUIT)
        {
            Engine::close = true;
        }
    }
    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        Object::updatePhysics(*Engine::objectsIterator);
    }
}

void Engine::render()
{
    SDL_RenderClear(Engine::renderer);
    float delta = Engine::timer->getTicks() / Engine::objects.size();
    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        (*Engine::objectsIterator)->render(delta);
    }
    SDL_RenderPresent(Engine::renderer);
}

bool Engine::shouldClose()
{
    return Engine::close;
}
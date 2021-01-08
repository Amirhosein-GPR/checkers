#include "core/Engine.hpp"

Engine * Engine::instance = nullptr;
SDL_Window * Engine::window = nullptr;
SDL_Renderer * Engine::renderer = nullptr;
SDL_Event * Engine::event = nullptr;
TTF_Font * Engine::font = nullptr;

std::vector<Object *> Engine::objects;
std::vector<Object *>::iterator Engine::objectsIterator;

bool Engine::running = true;
std::string Engine::username = "";
bool Engine::usernameSholudUpdate = false;
int Engine::usernameIndex = 3;

Engine::Engine()
{
    this->frameTimer = nullptr;
    this->mainTimer = nullptr;
    this->gameState = GAME_STATE_INTRO;
    this->firstMediaLoad = true;
}

Engine::~Engine()
{
    SDL_DestroyRenderer(Engine::renderer);
    SDL_DestroyWindow(Engine::window);

    Engine::renderer = nullptr;
    Engine::window = nullptr;

    deleteObjectsTillNow();

    delete Engine::event;
    delete Engine::frameTimer;
    delete Engine::mainTimer;
    delete Engine::instance;

    TTF_CloseFont(Engine::font);

    delete Engine::font;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Engine::deleteObjectsTillNow()
{
    for (Engine::objectsIterator = Engine::objects.end(); Engine::objectsIterator != Engine::objects.begin(); Engine::objectsIterator--)
    {
        std::cerr << "DELETED/";
        //delete *Engine::objectsIterator;
        Engine::objects.pop_back();
    }
    std::cerr << std::endl;
    Engine::objects.shrink_to_fit();
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
    Engine::frameTimer = new Timer();
    Engine::mainTimer = new Timer();
    return true;
}

void Engine::runThenGo(void (*function)(), Uint32 goToNextInThisTime, GameStateMachine gameState, bool lilimtedTime)
{
    if (this->firstMediaLoad)
    {
        this->deleteObjectsTillNow();
        function();
        this->firstMediaLoad = false;
    }

    if (this->mainTimer->getTicks() > goToNextInThisTime && lilimtedTime)
    {
        this->gameState = gameState;
        this->firstMediaLoad = true;
    }
    
}

void Engine::loadIntro()
{
    Object *introBackground = new Object(Vector(0, 0), Engine::renderer, Engine::font);
    introBackground->loadFromFile("assets/image/IntroBackground.png");
    Engine::objects.push_back(introBackground);

    Object *introText1 = new Object(Vector(0, 0));
    introText1->loadFromFile("assets/image/Texts.png");
    SDL_Rect *clip1 = new SDL_Rect {2, 3, 175, 70};
    introText1->setClip(clip1);
    introText1->setPosition((float) SCREEN_WIDTH / 2 - introText1->getWidth() / 2, 20);
    introText1->setAlpha(0);
    introText1->animate(255, 2);
    Engine::objects.push_back(introText1);

    Object *introText2 = new Object(Vector(500, 59));
    introText2->loadFromOtherObject(introText1);
    SDL_Rect *clip2 = new SDL_Rect {2, 59, 372, 70};
    introText2->setClip(clip2);
    introText2->setPosition((float) SCREEN_WIDTH / 2 - introText2->getWidth() / 2, 120);
    introText2->setAlpha(0);
    introText2->animate(255, 2);
    Engine::objects.push_back(introText2);

    Object *introText3 = new Object(Vector(500, 119));
    introText3->loadFromOtherObject(introText1);
    SDL_Rect *clip3 = new SDL_Rect {2, 119, 458, 70};
    introText3->setClip(clip3);
    introText3->setPosition((float) SCREEN_WIDTH / 2 - introText3->getWidth() / 2, 220);
    introText3->setAlpha(0);
    introText3->animate(255, 2);
    Engine::objects.push_back(introText3);

    Object *introText4 = new Object(Vector(500, 179));
    introText4->loadFromOtherObject(introText1);
    SDL_Rect *clip4 = new SDL_Rect {2, 179, 290, 70};
    introText4->setClip(clip4);
    introText4->setPosition((float) SCREEN_WIDTH / 2 - introText4->getWidth() / 2, 370);
    introText4->setAlpha(0);
    introText4->animate(255, 2);
    Engine::objects.push_back(introText4);

    Object *introText5 = new Object(Vector(500, 250));
    introText5->loadFromOtherObject(introText1);
    SDL_Rect *clip5 = new SDL_Rect {2, 250, 297, 53};
    introText5->setClip(clip5);
    introText5->setPosition((float) SCREEN_WIDTH / 2 - introText5->getWidth() / 2, 520);
    introText5->animate(0, 2, true);
    Engine::objects.push_back(introText5);

    Object *introText6 = new Object(Vector(500, 293));
    introText6->loadFromOtherObject(introText1);
    SDL_Rect *clip6 = new SDL_Rect {2, 293, 360, 53};
    introText6->setClip(clip6);
    introText6->setPosition((float) SCREEN_WIDTH - introText6->getWidth() - 20, (float) SCREEN_HEIGHT - 60);
    introText6->setAlpha(0);
    introText6->animate(255, 2);
    Engine::objects.push_back(introText6);

    Object *introText7 = new Object(Vector(500, 333));
    introText7->loadFromOtherObject(introText1);
    SDL_Rect *clip7 = new SDL_Rect {2, 333, 230, 53};
    introText7->setClip(clip7);
    introText7->setPosition(20, (float) SCREEN_HEIGHT - 60);
    introText7->setAlpha(0);
    introText7->animate(255, 2);
    Engine::objects.push_back(introText7);
}

void Engine::loadNameInput()
{
    Object *background = new Object(Vector(0, 0));
    background->loadFromFile("assets/image/InputBackground.png");
    Engine::objects.push_back(background);

    Object *input = new Object(Vector(0, 0));
    input->loadFromFile("assets/image/Input.png");
    input->setPosition((float) (SCREEN_WIDTH / 2)- input->getWidth() / 2, (float) (SCREEN_HEIGHT / 2) - input->getHeight() / 2);
    Engine::objects.push_back(input);

    Object *text = new Object(Vector(0, 0));
    text->loadFromFile("assets/image/Texts.png");
    SDL_Rect *inputClip = new SDL_Rect {2, 375, 335, 53};
    text->setClip(inputClip);
    text->setPosition((float) (SCREEN_WIDTH / 2) - text->getWidth() / 2, (float) (SCREEN_HEIGHT / 2) - text->getHeight() * 2);
    Engine::objects.push_back(text);

    Object *renderedText = new Object(Vector(0, 0));
    renderedText->loadFromRenderedText("-------------------------------", SDL_Color {0xFF, 0xFF, 0xFF});
    renderedText->setPosition((float) (SCREEN_WIDTH / 2)- input->getWidth() / 2 + 20, (float) (SCREEN_HEIGHT / 2) - input->getHeight() / 4);
    Engine::objects.push_back(renderedText);

    std::cerr << Engine::objects.size() << std::endl;
}

void Engine::loadMainGame()
{
    Object *infoBar = new Object(Vector(0, 0));
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
}

void Engine::loadOutro()
{}

void Engine::exitTheGame()
{}

void Engine::manageStates()
{
    std::cerr << Engine::gameState << "/" << GAME_STATE_INPUT << std::endl;
    switch (Engine::gameState)
    {
        case GAME_STATE_INTRO:
            Engine::runThenGo(Engine::loadIntro, 5000, GAME_STATE_INPUT);
        break;
        case GAME_STATE_INPUT:
            Engine::runThenGo(Engine::loadNameInput, 0, GAME_STATE_GAME, false);
        break;
        case GAME_STATE_GAME:
            Engine::runThenGo(Engine::loadMainGame, 0, GAME_STATE_OUTRO, false);
        break;
        case GAME_STATE_OUTRO:
            Engine::runThenGo(Engine::loadOutro, 0, GAME_STATE_EXIT, false);
        break;
        case GAME_STATE_EXIT:
            Engine::running = false;
        break;
    }
}

void Engine::update()
{
    while (SDL_PollEvent(Engine::event) != 0)
    {
        if (Engine::event->type == SDL_QUIT)
        {
            Engine::gameState = GAME_STATE_EXIT;
        }
        if (Engine::gameState == GAME_STATE_INPUT)
        {
            if (Engine::event->type == SDL_KEYDOWN)
            {
                switch (Engine::event->key.keysym.sym)
                {
                    case SDLK_RETURN:
                        Engine::gameState = GAME_STATE_GAME;
                        Engine::firstMediaLoad = true;
                    break;
                    case SDLK_BACKSPACE:
                        Engine::backspaceInputText();
                    break;
                    case SDLK_a:
                        Engine::appendInputText('a');
                    break;
                    case SDLK_b:
                        Engine::appendInputText('b');
                    break;
                    case SDLK_c:
                        Engine::appendInputText('c');
                    break;
                    case SDLK_d:
                        Engine::appendInputText('d');
                    break;
                    case SDLK_e:
                        Engine::appendInputText('e');
                    break;
                    case SDLK_f:
                        Engine::appendInputText('f');
                    break;
                    case SDLK_g:
                        Engine::appendInputText('g');
                    break;
                    case SDLK_h:
                        Engine::appendInputText('h');
                    break;
                    case SDLK_i:
                        Engine::appendInputText('i');
                    break;
                    case SDLK_j:
                        Engine::appendInputText('j');
                    break;
                    case SDLK_k:
                        Engine::appendInputText('k');
                    break;
                    case SDLK_l:
                        Engine::appendInputText('l');
                    break;
                    case SDLK_m:
                        Engine::appendInputText('m');
                    break;
                    case SDLK_n:
                        Engine::appendInputText('n');
                    break;
                    case SDLK_o:
                        Engine::appendInputText('o');
                    break;
                    case SDLK_p:
                        Engine::appendInputText('p');
                    break;
                    case SDLK_q:
                        Engine::appendInputText('q');
                    break;
                    case SDLK_r:
                        Engine::appendInputText('r');
                    break;
                    case SDLK_s:
                        Engine::appendInputText('s');
                    break;
                    case SDLK_t:
                        Engine::appendInputText('t');
                    break;
                    case SDLK_u:
                        Engine::appendInputText('u');
                    break;
                    case SDLK_v:
                        Engine::appendInputText('v');
                    break;
                    case SDLK_w:
                        Engine::appendInputText('w');
                    break;
                    case SDLK_x:
                        Engine::appendInputText('x');
                    break;
                    case SDLK_y:
                        Engine::appendInputText('y');
                    break;
                    case SDLK_z:
                        Engine::appendInputText('z');
                    break;
                    case SDLK_SPACE:
                        Engine::appendInputText(' ');
                    break;
                }
            }
        }
        
    }
    int i = 0;
    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        if (usernameSholudUpdate)
        {
            if (i == usernameIndex)
            {
                std::cerr << "YUP" << "username : " << username;
                (*Engine::objectsIterator)->loadFromRenderedText(Engine::username, SDL_Color {0xFF, 0xFF, 0xFF});
                (*Engine::objectsIterator)->setAlpha(255);
            }
        }
        
        Object::updatePhysics(*Engine::objectsIterator);
        i++;
    }
}

void Engine::render()
{
    SDL_RenderClear(Engine::renderer);
    float delta = Engine::frameTimer->getTicks() / Engine::objects.size();
    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        (*Engine::objectsIterator)->render(delta);
    }
    if (Engine::usernameSholudUpdate)
    {
        Engine::usernameSholudUpdate = false;
    }
    SDL_RenderPresent(Engine::renderer);
}

void Engine::appendInputText(const char &character)
{
    
    if (username.length() < 14)
    {
        if (username.size() == 0)
        {
            Engine::username += toupper(character);
        }
        else
        {
            Engine::username += character;
        }
        Engine::usernameSholudUpdate = true;
    }
}

void Engine::backspaceInputText()
{
    if (username.length() > 0)
    {
        Engine::username.pop_back();
        Engine::usernameSholudUpdate = true;
    }
}
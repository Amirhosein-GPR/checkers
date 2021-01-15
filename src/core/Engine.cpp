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
BoardCellState Engine::board[64];
Uint8 Engine::selectorBlockPosition = 56;
Uint8 Engine::attachedBlockIndex = 64;
Uint8 Engine::opponentAttachedBlockIndex = 64;
Uint8 Engine::shouldBeChangedBlockIndex = 64;
Uint8 Engine::opponentBlockPosition = 8;
bool Engine::usernameSholudUpdate = false;
bool Engine::boardSholudUpdate = false;
const int Engine::usernameIndex = 3;
const int Engine::selectorIndex = 39;
int Engine::attachedPieceID = -1;
bool Engine::pieceIDSholudUpdate = false;
bool Engine::redOrBlueTurn = false;
bool Engine::opponentShouldMove = false;
std::vector<Uint8> Engine::blueJumpOverIndice;
Uint8 Engine::startBlueJumpOverIndex = 64;
std::vector<Uint8> Engine::redJumpOverIndice;
Uint8 Engine::startRedJumpOverIndex = 64;
int Engine::userMoveCount = 0;
int Engine::opponentMoveCount = 0;
int Engine::userScore = 0;
int Engine::opponentScore = 0;
Mix_Chunk * Engine::typingChunk = nullptr;
Mix_Chunk * Engine::selectorMoveChunk = nullptr;
Mix_Chunk * Engine::attachChunk = nullptr;
Mix_Chunk * Engine::detttachChunk = nullptr;
Mix_Chunk * Engine::jumpOverChunk = nullptr;

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

    Mix_FreeChunk(Engine::typingChunk);
    Mix_FreeChunk(Engine::selectorMoveChunk);
    Mix_FreeChunk(Engine::attachChunk);
    Mix_FreeChunk(Engine::detttachChunk);
    Mix_FreeChunk(Engine::jumpOverChunk);

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
    for (int i = 0; i < 16; i++)
    {
        Engine::board[i] = BOARD_CELL_STATE_RED;
    }
    for (int i = 16; i < 48; i++)
    {
        Engine::board[i] = BOARD_CELL_STATE_EMPTY;
    }
    for (int i = 48; i < 64; i++)
    {
        Engine::board[i] = BOARD_CELL_STATE_BLUE;
    }

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
    Engine::typingChunk = Mix_LoadWAV("assets/sound/effect/Typing.mp3");
}

void Engine::loadMainGame()
{
    Object *infoBar = new Object(Vector(0, 0));
    infoBar->loadFromFile("assets/image/Info.png");
    Engine::objects.push_back(infoBar);

    Object *board = new Object(Vector(560, 0));
    board->loadFromFile("assets/image/Board.png");
    Engine::objects.push_back(board);

    Object *username = new Object(Vector(111, 115));
    username->loadFromRenderedText(Engine::username, SDL_Color {0xFF, 0xFF, 0xFF});
    Engine::objects.push_back(username);

    Object *userPoint = new Object(Vector(360, 263));
    userPoint->loadFromRenderedText("0", SDL_Color {0xFF, 0xFF, 0xFF});
    Engine::objects.push_back(userPoint);

    Object *opponenPoint = new Object(Vector(440, 337));
    opponenPoint->loadFromRenderedText("0", SDL_Color {0xFF, 0xFF, 0xFF});
    Engine::objects.push_back(opponenPoint);

    Object *numberOfUserMovements = new Object(Vector(162, 412));
    numberOfUserMovements->loadFromRenderedText("0", SDL_Color {0xFF, 0xFF, 0xFF});
    Engine::objects.push_back(numberOfUserMovements);

    Object *numberOfOpponentMovements = new Object(Vector(232, 481));
    numberOfOpponentMovements->loadFromRenderedText("0", SDL_Color {0xFF, 0xFF, 0xFF});
    Engine::objects.push_back(numberOfOpponentMovements);

    RedPiece *redPiece1 = new RedPiece(Vector(0, 0));
    redPiece1->loadFromFile("assets/image/Pieces.png");
    redPiece1->setBlockPosition(0);
    Engine::objects.push_back(redPiece1);

    for (int i = 1; i < 8; i++)
    {
        RedPiece *redPiece2To8 = new RedPiece(Vector(0, 0));
        redPiece2To8->loadFromOtherObject(redPiece1);
        redPiece2To8->setBlockPosition(i);
        Engine::objects.push_back(redPiece2To8);
    }
    
    for (int i = 8; i < 16; i++)
    {
        RedPiece *redPiece9To16 = new RedPiece(Vector(0, 0));
        redPiece9To16->loadFromOtherObject(redPiece1);
        redPiece9To16->setBlockPosition(i);
        Engine::objects.push_back(redPiece9To16);
    }
    
    for (int i = 48; i < 56; i++)
    {
        BluePiece *bluePiece1To8 = new BluePiece(Vector(0, 0));
        bluePiece1To8->loadFromOtherObject(redPiece1);
        bluePiece1To8->setBlockPosition(i);
        Engine::objects.push_back(bluePiece1To8);
    }
    
    for (int i = 56; i < 64; i++)
    {
        BluePiece *bluePiece9To16 = new BluePiece(Vector(0, 0));
        bluePiece9To16->loadFromOtherObject(redPiece1);
        bluePiece9To16->setBlockPosition(i);
        Engine::objects.push_back(bluePiece9To16);
    }

    Selector *selector = new Selector(Vector(0, 0));
    selector->loadFromFile("assets/image/Selector.png");
    selector->setBlockPosition(56);
    Engine::objects.push_back(selector);

    Engine::selectorMoveChunk = Mix_LoadWAV("assets/sound/effect/Move.mp3");
    Engine::attachChunk = Mix_LoadWAV("assets/sound/effect/Attach.mp3");
    Engine::detttachChunk = Mix_LoadWAV("assets/sound/effect/Dettach.mp3");
    Engine::jumpOverChunk = Mix_LoadWAV("assets/sound/effect/JumpOver.mp3");
    //rr
}

void Engine::loadOutro()
{}

void Engine::exitTheGame()
{}

void Engine::manageStates()
{
    switch (Engine::gameState)
    {
        case GAME_STATE_INTRO:
            Engine::runThenGo(Engine::loadIntro, 1000, GAME_STATE_INPUT);
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
        if (Engine::event->type == SDL_KEYDOWN)
        {
            switch (Engine::gameState)
            {
                case GAME_STATE_INPUT:
                    switch (Engine::event->key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            Engine::gameState = GAME_STATE_GAME;
                            Engine::firstMediaLoad = true;
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_BACKSPACE:
                            Engine::backspaceInputText();
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_a:
                            Engine::appendInputText('a');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_b:
                            Engine::appendInputText('b');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_c:
                            Engine::appendInputText('c');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_d:
                            Engine::appendInputText('d');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_e:
                            Engine::appendInputText('e');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_f:
                            Engine::appendInputText('f');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_g:
                            Engine::appendInputText('g');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_h:
                            Engine::appendInputText('h');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_i:
                            Engine::appendInputText('i');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_j:
                            Engine::appendInputText('j');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_k:
                            Engine::appendInputText('k');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_l:
                            Engine::appendInputText('l');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_m:
                            Engine::appendInputText('m');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_n:
                            Engine::appendInputText('n');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_o:
                            Engine::appendInputText('o');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_p:
                            Engine::appendInputText('p');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_q:
                            Engine::appendInputText('q');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_r:
                            Engine::appendInputText('r');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_s:
                            Engine::appendInputText('s');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_t:
                            Engine::appendInputText('t');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_u:
                            Engine::appendInputText('u');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_v:
                            Engine::appendInputText('v');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_w:
                            Engine::appendInputText('w');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_x:
                            Engine::appendInputText('x');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_y:
                            Engine::appendInputText('y');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_z:
                            Engine::appendInputText('z');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                        case SDLK_SPACE:
                            Engine::appendInputText(' ');
                            if (!Mix_Playing(-1))
                            {
                                Mix_PlayChannel(-1, Engine::typingChunk, 0);
                            }
                        break;
                    }
                break;
                case GAME_STATE_GAME:
                    switch (Engine::event->key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if (Engine::board[Engine::selectorBlockPosition] == BOARD_CELL_STATE_BLUE && Engine::attachedPieceID == -1)
                            {
                                this->checkBluePieceJumpOver();
                                if (Engine::blueJumpOverIndice.size())
                                {
                                    if (Engine::startBlueJumpOverIndex == 64)
                                    {
                                        for (int i = 0; i < Engine::blueJumpOverIndice.size(); i++)
                                        {
                                            if (Engine::selectorBlockPosition == Engine::blueJumpOverIndice.at(i))
                                            {
                                                Engine::startBlueJumpOverIndex = Engine::selectorBlockPosition;
                                                Engine::attachedBlockIndex = Engine::selectorBlockPosition;
                                                Engine::pieceIDSholudUpdate = true;
                                                Mix_PlayChannel(-1, Engine::attachChunk, 0);
                                            }
                                        }
                                    }
                                }
                                else if (this->pieceSimpleJump())
                                {
                                    Engine::attachedBlockIndex = Engine::selectorBlockPosition;
                                    Engine::pieceIDSholudUpdate = true;
                                    Mix_PlayChannel(-1, Engine::attachChunk, 0);
                                }
                            }
                            else if (Engine::board[Engine::selectorBlockPosition] == BOARD_CELL_STATE_EMPTY && Engine::attachedPieceID != -1)
                            {
                                if (Engine::startBlueJumpOverIndex != 64)
                                {
                                    int distance = Engine::startBlueJumpOverIndex - Engine::selectorBlockPosition;
                                    if (distance == 14 || 18)
                                    {
                                        if (Engine::startBlueJumpOverIndex % 8 == 0)
                                        {
                                            if (Engine::board[Engine::startBlueJumpOverIndex - 9] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startBlueJumpOverIndex - 18] == BOARD_CELL_STATE_EMPTY)
                                            {
                                                Engine::objects.at(Engine::startBlueJumpOverIndex)->setBlockPosition(Engine::startBlueJumpOverIndex - 18);
                                                int j;
                                                for (j = 7; j < 23; j++)
                                                {
                                                    if (Engine::objects.at(j)->getBlockPosition() == Engine::startBlueJumpOverIndex - 9)
                                                    {
                                                        break;
                                                    }
                                                }
                                                BluePiece *bluePiece = new BluePiece(Vector(0, 0));
                                                bluePiece->loadFromOtherObject(Engine::objects.at(7));
                                                bluePiece->setBlockPosition(Engine::startBlueJumpOverIndex - 9);
                                                Engine::objects.at(j) = bluePiece;
                                                Engine::board[Engine::startBlueJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                                Engine::board[Engine::startBlueJumpOverIndex - 9] = BOARD_CELL_STATE_RED;
                                                Engine::board[Engine::startBlueJumpOverIndex - 18] = BOARD_CELL_STATE_RED;
                                                Engine::userMoveCount += 1;
                                                Engine::userScore += 10;
                                                Mix_PlayChannel(-1, Engine::jumpOverChunk, 0);
                                                break;
                                            }
                                        }
                                        else if (Engine::startBlueJumpOverIndex == 7 || Engine::startBlueJumpOverIndex == 15 || Engine::startBlueJumpOverIndex == 23 || Engine::startBlueJumpOverIndex == 31 || Engine::startBlueJumpOverIndex == 39 || Engine::startBlueJumpOverIndex == 47 || Engine::startBlueJumpOverIndex == 55 || Engine::startBlueJumpOverIndex == 63)
                                        {
                                            if (Engine::board[Engine::startBlueJumpOverIndex - 7] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startBlueJumpOverIndex - 14] == BOARD_CELL_STATE_EMPTY)
                                            {
                                                Engine::objects.at(Engine::startBlueJumpOverIndex)->setBlockPosition(Engine::startBlueJumpOverIndex - 14);
                                                int j;
                                                for (j = 7; j < 23; j++)
                                                {
                                                    if (Engine::objects.at(j)->getBlockPosition() == Engine::startBlueJumpOverIndex - 7)
                                                    {
                                                        break;
                                                    }
                                                }
                                                BluePiece *bluePiece = new BluePiece(Vector(0, 0));
                                                bluePiece->loadFromOtherObject(Engine::objects.at(7));
                                                bluePiece->setBlockPosition(Engine::startBlueJumpOverIndex - 7);
                                                Engine::objects.at(j) = bluePiece;
                                                Engine::board[Engine::startBlueJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                                Engine::board[Engine::startBlueJumpOverIndex - 7] = BOARD_CELL_STATE_RED;
                                                Engine::board[Engine::startBlueJumpOverIndex - 14] = BOARD_CELL_STATE_RED;
                                                Engine::userMoveCount += 1;
                                                Engine::userScore += 10;
                                                Mix_PlayChannel(-1, Engine::jumpOverChunk, 0);
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            if (Engine::board[Engine::startBlueJumpOverIndex - 9] == BOARD_CELL_STATE_RED && Engine::board[Engine::startBlueJumpOverIndex - 18] == BOARD_CELL_STATE_EMPTY)
                                            {
                                                Engine::objects.at(Engine::startBlueJumpOverIndex)->setBlockPosition(Engine::startBlueJumpOverIndex - 18);
                                                int j;
                                                for (j = 7; j < 23; j++)
                                                {
                                                    if (Engine::objects.at(j)->getBlockPosition() == Engine::startBlueJumpOverIndex - 9)
                                                    {
                                                        break;
                                                    }
                                                }
                                                BluePiece *bluePiece = new BluePiece(Vector(0, 0));
                                                bluePiece->loadFromOtherObject(Engine::objects.at(7));
                                                bluePiece->setBlockPosition(Engine::startBlueJumpOverIndex - 9);
                                                Engine::objects.at(j) = bluePiece;
                                                Engine::board[Engine::startBlueJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                                Engine::board[Engine::startBlueJumpOverIndex - 9] = BOARD_CELL_STATE_BLUE;
                                                Engine::board[Engine::startBlueJumpOverIndex - 18] = BOARD_CELL_STATE_BLUE;
                                                Engine::pieceIDSholudUpdate = false;
                                                Engine::attachedPieceID = -1;
                                                Engine::boardSholudUpdate = true;
                                                Engine::opponentShouldMove = true;
                                                Engine::redOrBlueTurn = !Engine::redOrBlueTurn;
                                                Engine::userMoveCount += 1;
                                                Engine::userScore += 10;
                                                Mix_PlayChannel(-1, Engine::jumpOverChunk, 0);
                                                break;
                                            }
                                            else if (Engine::board[Engine::startBlueJumpOverIndex - 7] == BOARD_CELL_STATE_RED && Engine::board[Engine::startBlueJumpOverIndex - 14] == BOARD_CELL_STATE_EMPTY)
                                            {
                                                Engine::objects.at(Engine::startBlueJumpOverIndex)->setBlockPosition(Engine::startBlueJumpOverIndex + 14);
                                                int j;
                                                for (j = 7; j < 23; j++)
                                                {
                                                    if (Engine::objects.at(j)->getBlockPosition() == Engine::startBlueJumpOverIndex + 7)
                                                    {
                                                        break;
                                                    }
                                                }
                                                BluePiece *bluePiece = new BluePiece(Vector(0, 0));
                                                bluePiece->loadFromOtherObject(Engine::objects.at(7));
                                                bluePiece->setBlockPosition(Engine::startBlueJumpOverIndex - 7);
                                                Engine::objects.at(j) = bluePiece;
                                                Engine::board[Engine::startBlueJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                                Engine::board[Engine::startBlueJumpOverIndex - 7] = BOARD_CELL_STATE_BLUE;
                                                Engine::board[Engine::startBlueJumpOverIndex - 14] = BOARD_CELL_STATE_BLUE;
                                                Engine::pieceIDSholudUpdate = false;
                                                Engine::attachedPieceID = -1;
                                                Engine::boardSholudUpdate = true;
                                                Engine::opponentShouldMove = true;
                                                Engine::redOrBlueTurn = !Engine::redOrBlueTurn;
                                                Engine::userMoveCount += 1;
                                                Engine::userScore += 10;
                                                Mix_PlayChannel(-1, Engine::jumpOverChunk, 0);
                                                break;
                                            }
                                        }
                                        Engine::startBlueJumpOverIndex = 64;
                                    }
                                }
                                else if (this->dettachBlue())
                                {
                                    board[Engine::attachedBlockIndex] = BOARD_CELL_STATE_EMPTY;
                                    Engine::board[Engine::selectorBlockPosition] = BOARD_CELL_STATE_BLUE;
                                    Engine::pieceIDSholudUpdate = false;
                                    Engine::attachedPieceID = -1;
                                    Engine::boardSholudUpdate = true;
                                    Engine::opponentShouldMove = true;
                                    Engine::redOrBlueTurn = !Engine::redOrBlueTurn;
                                    Engine::userMoveCount += 1;
                                    Mix_PlayChannel(-1, Engine::detttachChunk, 0);
                                }
                            }
                        break;
                        case SDLK_UP:
                            this->moveSelector(-8);
                        break;
                        case SDLK_RIGHT:
                            this->moveSelector(1);
                        break;
                        case SDLK_DOWN:
                            this->moveSelector(8);
                        break;
                        case SDLK_LEFT:
                            this->moveSelector(-1);
                        break;
                    }
                    
                    if (Engine::opponentShouldMove)
                    {
                        do
                        {
                            this->checkRedPiecesJumpOver();
                            if (Engine::redJumpOverIndice.size())
                            {
                                for (int i = 0; i < Engine::redJumpOverIndice.size(); i++)
                                {
                                    Engine::startRedJumpOverIndex = Engine::redJumpOverIndice.at(i);
                                    break;
                                }
                            }
                            if (Engine::redOrBlueTurn)
                            {
                                if (Engine::startRedJumpOverIndex == 64)
                                {
                                    Uint8 redPieceIndex;
                                    redPieceIndex = rand() % 16 + 7;
                                    Engine::opponentBlockPosition = Engine::objects.at(redPieceIndex)->getBlockPosition();
                                    if (Engine::opponentBlockPosition % 8 == 0)
                                    {
                                        if (Engine::board[Engine::opponentBlockPosition + 9] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::opponentBlockPosition + 9);
                                            Engine::objects.at(redPieceIndex) = redPiece;
                                            Engine::board[Engine::opponentBlockPosition] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::opponentBlockPosition + 9] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            break;
                                        }
                                    }
                                    else if (Engine::opponentBlockPosition == 7 || Engine::opponentBlockPosition == 15 || Engine::opponentBlockPosition == 23 || Engine::opponentBlockPosition == 31 || Engine::opponentBlockPosition == 39 || Engine::opponentBlockPosition == 47 || Engine::opponentBlockPosition == 55 || Engine::opponentBlockPosition == 63)
                                    {
                                        if (Engine::board[Engine::opponentBlockPosition + 7] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::opponentBlockPosition + 7);
                                            Engine::objects.at(redPieceIndex) = redPiece;
                                            Engine::board[Engine::opponentBlockPosition] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::opponentBlockPosition + 7] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (Engine::board[Engine::opponentBlockPosition + 9] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::opponentBlockPosition + 9);
                                            Engine::objects.at(redPieceIndex) = redPiece;
                                            Engine::board[Engine::opponentBlockPosition] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::opponentBlockPosition + 9] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            break;
                                        }
                                        else if (Engine::board[Engine::opponentBlockPosition + 7] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::opponentBlockPosition + 7);
                                            Engine::objects.at(redPieceIndex) = redPiece;
                                            Engine::board[Engine::opponentBlockPosition] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::opponentBlockPosition + 7] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    int i;
                                    for (i = 7; i < 23; i++)
                                    {
                                        if (Engine::objects.at(i)->getBlockPosition() == Engine::startRedJumpOverIndex)
                                        {
                                            break;
                                        }
                                    }

                                    if (Engine::startRedJumpOverIndex % 8 == 0)
                                    {
                                        if (Engine::board[Engine::startRedJumpOverIndex + 9] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startRedJumpOverIndex + 18] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            Engine::objects.at(i)->setBlockPosition(Engine::startRedJumpOverIndex + 18);
                                            int j;
                                            for (j = 23; j < 39; j++)
                                            {
                                                if (Engine::objects.at(j)->getBlockPosition() == Engine::startRedJumpOverIndex + 9)
                                                {
                                                    break;
                                                }
                                            }
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::startRedJumpOverIndex + 9);
                                            Engine::objects.at(j) = redPiece;
                                            Engine::board[Engine::startRedJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::startRedJumpOverIndex + 9] = BOARD_CELL_STATE_RED;
                                            Engine::board[Engine::startRedJumpOverIndex + 18] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            Engine::opponentScore += 10;
                                            break;
                                        }
                                    }
                                    else if (Engine::startRedJumpOverIndex == 7 || Engine::startRedJumpOverIndex == 15 || Engine::startRedJumpOverIndex == 23 || Engine::startRedJumpOverIndex == 31 || Engine::startRedJumpOverIndex == 39 || Engine::startRedJumpOverIndex == 47 || Engine::startRedJumpOverIndex == 55 || Engine::startRedJumpOverIndex == 63)
                                    {
                                        if (Engine::board[Engine::startRedJumpOverIndex + 7] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startRedJumpOverIndex + 14] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            Engine::objects.at(i)->setBlockPosition(Engine::startRedJumpOverIndex + 14);
                                            int j;
                                            for (j = 23; j < 39; j++)
                                            {
                                                if (Engine::objects.at(j)->getBlockPosition() == Engine::startRedJumpOverIndex + 7)
                                                {
                                                    break;
                                                }
                                            }
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::startRedJumpOverIndex + 7);
                                            Engine::objects.at(j) = redPiece;
                                            Engine::board[Engine::startRedJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::startRedJumpOverIndex + 7] = BOARD_CELL_STATE_RED;
                                            Engine::board[Engine::startRedJumpOverIndex + 14] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            Engine::opponentScore += 10;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (Engine::board[Engine::startRedJumpOverIndex + 9] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startRedJumpOverIndex + 18] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            Engine::objects.at(i)->setBlockPosition(Engine::startRedJumpOverIndex + 18);
                                            int j;
                                            for (j = 23; j < 39; j++)
                                            {
                                                if (Engine::objects.at(j)->getBlockPosition() == Engine::startRedJumpOverIndex + 9)
                                                {
                                                    break;
                                                }
                                            }
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::startRedJumpOverIndex + 9);
                                            Engine::objects.at(j) = redPiece;
                                            Engine::board[Engine::startRedJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::startRedJumpOverIndex + 9] = BOARD_CELL_STATE_RED;
                                            Engine::board[Engine::startRedJumpOverIndex + 18] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            Engine::opponentScore += 10;
                                            break;
                                        }
                                        else if (Engine::board[Engine::startRedJumpOverIndex + 7] == BOARD_CELL_STATE_BLUE && Engine::board[Engine::startRedJumpOverIndex + 14] == BOARD_CELL_STATE_EMPTY)
                                        {
                                            Engine::objects.at(i)->setBlockPosition(Engine::startRedJumpOverIndex + 14);
                                            int j;
                                            for (j = 23; j < 39; j++)
                                            {
                                                if (Engine::objects.at(j)->getBlockPosition() == Engine::startRedJumpOverIndex + 7)
                                                {
                                                    std::cerr << "FOUND : " << j << " : " << (int) Engine::objects.at(j)->getBlockPosition() << "/" << (int) Engine::startRedJumpOverIndex + 7;
                                                    break;
                                                }
                                            }
                                            RedPiece *redPiece = new RedPiece(Vector(0, 0));
                                            redPiece->loadFromOtherObject(Engine::objects.at(7));
                                            redPiece->setBlockPosition(Engine::startRedJumpOverIndex + 7);
                                            Engine::objects.at(j) = redPiece;
                                            Engine::board[Engine::startRedJumpOverIndex] = BOARD_CELL_STATE_EMPTY;
                                            Engine::board[Engine::startRedJumpOverIndex + 7] = BOARD_CELL_STATE_RED;
                                            Engine::board[Engine::startRedJumpOverIndex + 14] = BOARD_CELL_STATE_RED;
                                            Engine::opponentMoveCount += 1;
                                            Engine::opponentScore += 10;
                                            break;
                                        }
                                    }
                                    Engine::startRedJumpOverIndex = 64;
                                }
                            }
                        }
                        while(true);
                        Engine::redOrBlueTurn = !Engine::redOrBlueTurn;
                    }

                    Engine::opponentShouldMove = false;

                break;
            }
        }
    }

    int i = 0;
    for (Engine::objectsIterator = Engine::objects.begin(); Engine::objectsIterator != Engine::objects.end(); Engine::objectsIterator++)
    {
        if (Engine::usernameSholudUpdate || Engine::boardSholudUpdate)
        {
            switch (i)
            {
                case Engine::usernameIndex:
                    if (Engine::gameState == GAME_STATE_INPUT)
                    {
                        (*Engine::objectsIterator)->loadFromRenderedText(Engine::username, SDL_Color {0xFF, 0xFF, 0xFF});
                        (*Engine::objectsIterator)->setAlpha(255);
                    }
                    else if (Engine::gameState == GAME_STATE_GAME)
                    {
                        Engine::objects.at(3)->loadFromRenderedText(std::to_string(Engine::userMoveCount), SDL_Color {0xFF, 0xFF, 0xFF});
                        Engine::objects.at(3)->setAlpha(255);
                        Engine::objects.at(4)->loadFromRenderedText(std::to_string(Engine::opponentMoveCount), SDL_Color {0xFF, 0xFF, 0xFF});
                        Engine::objects.at(4)->setAlpha(255);
                        Engine::objects.at(5)->loadFromRenderedText(std::to_string(Engine::userScore), SDL_Color {0xFF, 0xFF, 0xFF});
                        Engine::objects.at(5)->setAlpha(255);
                        Engine::objects.at(6)->loadFromRenderedText(std::to_string(Engine::opponentScore), SDL_Color {0xFF, 0xFF, 0xFF});
                        Engine::objects.at(6)->setAlpha(255);
                    }
                break;
                case Engine::selectorIndex:
                    Engine::boardSholudUpdate = false;
                    (*Engine::objectsIterator)->setBlockPosition(Engine::selectorBlockPosition);
                break;
            }
            if (Engine::pieceIDSholudUpdate && (*Engine::objectsIterator)->getBlockPosition() == Engine::attachedBlockIndex)
            {
                Engine::attachedPieceID = i;
                (*Engine::objectsIterator)->setBlockPosition(Engine::selectorBlockPosition);
                Engine::pieceIDSholudUpdate = false;
            }
        }
        
        Object::updatePhysics(*Engine::objectsIterator);
        i++;
    }
    if (Engine::attachedPieceID != -1)
    {
        Engine::objects.at(Engine::attachedPieceID)->setBlockPosition(Engine::selectorBlockPosition);
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

void Engine::moveSelector(Uint8 numberOfBlocks)
{
    Mix_PlayChannel(-1, Engine::selectorMoveChunk, 0);
    Engine::selectorBlockPosition += numberOfBlocks;
    Engine::boardSholudUpdate = true;
    if (Engine::selectorBlockPosition < 0 || Engine::selectorBlockPosition > 63)
    {
        Engine::selectorBlockPosition -= numberOfBlocks;
    }
}

void Engine::checkBluePieceJumpOver()
{
    Engine::blueJumpOverIndice.clear();
    if (!Engine::redOrBlueTurn)
    {
        for (int i = 23; i < 39; i++)
        {
            Uint8 currentBlockPosition = Engine::objects.at(i)->getBlockPosition();
            if (currentBlockPosition % 8 == 0)
            {
                if (Engine::board[currentBlockPosition - 7] == BOARD_CELL_STATE_RED && Engine::board[currentBlockPosition - 14] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::blueJumpOverIndice.push_back(Engine::selectorBlockPosition);
                }
            }
            else if (currentBlockPosition == 7 || currentBlockPosition == 15 || currentBlockPosition == 23 || currentBlockPosition == 31 || currentBlockPosition == 39 || currentBlockPosition == 47 || currentBlockPosition == 55 || currentBlockPosition == 63)
            {
                if (Engine::board[currentBlockPosition - 9] == BOARD_CELL_STATE_RED && Engine::board[currentBlockPosition - 18] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::blueJumpOverIndice.push_back(Engine::selectorBlockPosition);
                }
            }
            else
            {
                if (Engine::board[currentBlockPosition - 7] == BOARD_CELL_STATE_RED && Engine::board[currentBlockPosition - 14] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::blueJumpOverIndice.push_back(Engine::selectorBlockPosition);
                }
                if (Engine::board[currentBlockPosition - 9] == BOARD_CELL_STATE_RED && Engine::board[currentBlockPosition - 18] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::blueJumpOverIndice.push_back(Engine::selectorBlockPosition);
                }
            }
        }
    }
}

bool Engine::pieceSimpleJump()
{
    if (Engine::redOrBlueTurn)
    {
        if (Engine::opponentBlockPosition % 8 == 0)
        {
            if (Engine::board[Engine::opponentBlockPosition + 7] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
        else if (Engine::opponentBlockPosition == 7 || Engine::opponentBlockPosition == 15 || Engine::opponentBlockPosition == 23 || Engine::opponentBlockPosition == 31 || Engine::opponentBlockPosition == 39 || Engine::opponentBlockPosition == 47 || Engine::opponentBlockPosition == 55 || Engine::opponentBlockPosition == 63)
        {
            if (Engine::board[Engine::opponentBlockPosition + 9] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
        else
        {
            if (Engine::board[Engine::opponentBlockPosition + 7] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
            else if (Engine::board[Engine::opponentBlockPosition + 9] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
    }
    else
    {
        if (Engine::selectorBlockPosition % 8 == 0)
        {
            if (Engine::board[Engine::selectorBlockPosition - 7] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
        else if (Engine::selectorBlockPosition == 7 || Engine::selectorBlockPosition == 15 || Engine::selectorBlockPosition == 23 || Engine::selectorBlockPosition == 31 || Engine::selectorBlockPosition == 39 || Engine::selectorBlockPosition == 47 || Engine::selectorBlockPosition == 55 || Engine::selectorBlockPosition == 63)
        {
            if (Engine::board[Engine::selectorBlockPosition - 9] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
        else
        {
            if (Engine::board[Engine::selectorBlockPosition - 7] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
            else if (Engine::board[Engine::selectorBlockPosition - 9] == BOARD_CELL_STATE_EMPTY)
            {
                return true;
            }
        }
    }
    return false;
}

void Engine::checkRedPiecesJumpOver()
{
    Engine::redJumpOverIndice.clear();
    if (Engine::redOrBlueTurn)
    {
        for (int i = 7; i < 23; i++)
        {
            Uint8 currentBlockPosition = Engine::objects.at(i)->getBlockPosition();
            if (currentBlockPosition % 8 == 0)
            {
                if (Engine::board[currentBlockPosition + 9] == BOARD_CELL_STATE_BLUE && Engine::board[currentBlockPosition + 18] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::redJumpOverIndice.push_back(currentBlockPosition);
                }
            }
            else if (currentBlockPosition == 7 || currentBlockPosition == 15 || currentBlockPosition == 23 || currentBlockPosition == 31 || currentBlockPosition == 39 || currentBlockPosition == 47 || currentBlockPosition == 55 || currentBlockPosition == 63)
            {
                if (Engine::board[currentBlockPosition + 7] == BOARD_CELL_STATE_BLUE && Engine::board[currentBlockPosition + 14] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::redJumpOverIndice.push_back(currentBlockPosition);
                }
            }
            else
            {
                if (Engine::board[currentBlockPosition + 9] == BOARD_CELL_STATE_BLUE && Engine::board[currentBlockPosition + 18] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::redJumpOverIndice.push_back(currentBlockPosition);
                }
                if (Engine::board[currentBlockPosition + 7] == BOARD_CELL_STATE_BLUE && Engine::board[currentBlockPosition + 14] == BOARD_CELL_STATE_EMPTY)
                {
                    Engine::redJumpOverIndice.push_back(currentBlockPosition);
                }
            }
        }
    }
}

bool Engine::dettachBlue()
{
    int distance;
    if (Engine::startBlueJumpOverIndex != 64)
    {
        distance = Engine::startBlueJumpOverIndex - Engine::selectorBlockPosition;
        if (distance == 18 || distance == 14)
        {
            Engine::startBlueJumpOverIndex = 64;
            return true;
        }
    }
    else
    {
        distance = Engine::attachedBlockIndex - Engine::selectorBlockPosition;
        if (Engine::attachedBlockIndex % 8 == 0)
        {
            if (distance == 7)
            {
                return true;
            }
        }
        else if (Engine::attachedBlockIndex == 7 || Engine::attachedBlockIndex == 15 || Engine::attachedBlockIndex == 23 || Engine::attachedBlockIndex == 31 || Engine::attachedBlockIndex == 39 || Engine::attachedBlockIndex == 47 || Engine::attachedBlockIndex == 55 || Engine::attachedBlockIndex == 63)
        {
            if (distance == 9)
            {
                return true;
            }
        }
        else
        {
            if (distance == 7)
            {
                return true;
            }
            else if (distance == 9)
            {
                return true;
            }
        }
    }
    return false;
}
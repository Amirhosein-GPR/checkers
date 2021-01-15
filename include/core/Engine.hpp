#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/System.hpp"
#include "core/Object.hpp"
#include "objects/Piece.hpp"
#include "objects/RedPiece.hpp"
#include "objects/BluePiece.hpp"
#include "objects/Selector.hpp"

enum GameStateMachine
{
    GAME_STATE_INTRO,
    GAME_STATE_INPUT,
    GAME_STATE_GAME,
    GAME_STATE_OUTRO,
    GAME_STATE_EXIT
};

enum BoardCellState
{
    BOARD_CELL_STATE_RED,
    BOARD_CELL_STATE_EMPTY,
    BOARD_CELL_STATE_BLUE
};

enum PieceJumpState
{
    PIECE_JUMP_STATE_SIMPLE,
    PIECE_JUMP_STATE_OVER,
    PIECE_JUMP_STATE_NO_JUMP
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
        static BoardCellState board[64];
        static Uint8 selectorBlockPosition;
        static Uint8 attachedBlockIndex;
        static Uint8 opponentAttachedBlockIndex;
        static Uint8 shouldBeChangedBlockIndex;
        static Uint8 opponentBlockPosition;
        static bool usernameSholudUpdate;
        static bool boardSholudUpdate;
        static const int usernameIndex;
        static const int selectorIndex;
        static int attachedPieceID;
        static bool pieceIDSholudUpdate;
        static bool redOrBlueTurn;
        static bool opponentShouldMove;
        static std::vector<Uint8> blueJumpOverIndice;
        static Uint8 startBlueJumpOverIndex;
        static std::vector<Uint8> redJumpOverIndice;
        static Uint8 startRedJumpOverIndex;
        static int userMoveCount;
        static int opponentMoveCount;
        static int userScore;
        static int opponentScore;
        static Mix_Chunk *typingChunk;
        static Mix_Chunk *selectorMoveChunk;
        static Mix_Chunk *attachChunk;
        static Mix_Chunk *detttachChunk;
        static Mix_Chunk *jumpOverChunk;
        static void appendInputText(const char &character);
        static void backspaceInputText();
        void moveSelector(Uint8 numberOfBlocks);
        void checkBluePieceJumpOver();
        bool pieceSimpleJump();
        void checkRedPiecesJumpOver();
        bool dettachBlue();
};

#endif
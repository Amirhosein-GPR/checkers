#include "core/Engine.hpp"

int main(int argc, char *argv[])
{
    Engine *engine = Engine::getInstance();

    if (engine->initialize())
    {
        if (engine->loadMedia())
        {
            Uint32 frameTicks;
            while (true)
            {
                Engine::timer->start();
                engine->update();
                engine->render();
                
                if (engine->shouldClose())
                {
                    break;
                }

                frameTicks = Engine::timer->getTicks();
                if (frameTicks < totalTicksForEachFrame)
                {
                    std::cerr << "A";
                    SDL_Delay(totalTicksForEachFrame - frameTicks);
                }
            }
        }
        else
        {
            System::logError("engine : couldn't load media!");
        }
    }
    else
    {
        System::logError("engine : couldn't initialize!");
    }

    return 0;
}
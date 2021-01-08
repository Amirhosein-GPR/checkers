#include "core/Engine.hpp"

void func()
{
    
}

int main(int argc, char *argv[])
{
    Engine *engine = Engine::getInstance();

    if (engine->initialize())
    {
            Uint32 frameTicks;
            engine->mainTimer->start();
            while (Engine::running)
            {
                engine->manageStates();
                engine->frameTimer->start();
                engine->update();
                engine->render();

                frameTicks = engine->frameTimer->getTicks();
                if (frameTicks < totalTicksForEachFrame)
                {
                    SDL_Delay(totalTicksForEachFrame - frameTicks);
                }
            }
            engine->deleteObjectsTillNow();
    }
    else
    {
        System::logError("engine : couldn't initialize!");
    }

    return 0;
}
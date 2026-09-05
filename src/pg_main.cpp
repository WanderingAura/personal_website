
#include "raylib.h"
#include "pg_game.hpp"
#include "pg_update_frame.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int screenWidth = 800;
int screenHeight = 450;

int main()
{
    InitWindow(screenWidth, screenHeight, "Personal Gallery");

    SetExitKey(KEY_NULL);

    DisableCursor();
    PG::Game::Initialise();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(PG::UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose() && !PG::ShouldQuit())
    {
        PG::UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}

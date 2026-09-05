
#include "raylib.h"
#include "pg_game.hpp"
#include "pg_update_frame.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int screenWidth = 800;
int screenHeight = 450;


void UpdateDrawFrame(void);     // Update and Draw one frame

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Personal Gallery");

    // ESC is used to close the boids params menu, not just to quit; the default exit key
    // would quit unconditionally on Esc regardless of the menu, so it's disabled here and
    // PG::ShouldQuit() (below) does the equivalent check but only when the menu isn't open
    SetExitKey(KEY_NULL);

    DisableCursor();
    PG::Game::Initialise();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(PG::UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose() && !PG::ShouldQuit())    // Detect window close button or Esc (see PG::ShouldQuit)
    {
        PG::UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

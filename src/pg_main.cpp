
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

    DisableCursor();
    PG::Game::Initialise();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(PG::UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
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

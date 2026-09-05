#include "raylib.h"
#include "pg_game.hpp"
#include "pg_cone.hpp"
#include "pg_crosshair.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

static bool quitRequested = false;

void UpdateDrawFrame(void)
{
    Game& game = Game::getInstance();

    // while the boids params menu is open it owns the now-visible cursor, so camera
    // look/movement and its keybinds must be frozen
    bool menuOpen = game.boidsDisplay.IsMenuOpen();

    // Esc closes the menu (handled inside boidsDisplay.Update below) when it's open; only
    // treat it as "quit" when the menu isn't around to consume it (see SetExitKey(KEY_NULL))
    if (!menuOpen && IsKeyPressed(KEY_ESCAPE))
    {
        quitRequested = true;
    }

    if (!menuOpen)
    {
        UpdateCamera(&game.cam, CAMERA_FREE);
    }
    game.boidsDisplay.Update(game.cam);
    game.lighting.Update(game.cam);

    if (!menuOpen && IsKeyPressed(KEY_Z)) game.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(game.cam);
            game.lighting.BeginMode();
                game.room.Draw();
            game.lighting.EndMode();
            game.boidsDisplay.Draw();
        EndMode3D();

        if (!menuOpen)
        {
            Crosshair::Draw();
        }
        game.boidsDisplay.DrawTooltip();
        game.boidsDisplay.DrawMenu();

        // DrawRectangle( 10, 10, 320, 93, Fade(SKYBLUE, 0.5f) );
        // DrawRectangleLines( 10, 10, 320, 93, BLUE);

        // DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        // DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        // DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        // DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);
        DrawFPS(300, 0);

    EndDrawing();
}

bool ShouldQuit(void)
{
    return quitRequested;
}

}
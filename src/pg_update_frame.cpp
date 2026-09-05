#include "raylib.h"
#include "pg_game.hpp"
#include "pg_cone.hpp"
#include "pg_crosshair.hpp"
#include "pg_resume_prompt.hpp"
#include <raymath.h>
#include <rlgl.h>

#if defined(PLATFORM_WEB)
#include <emscripten/html5.h>
#endif

namespace PG
{

static bool quitRequested = false;

// Web only: true while waiting for a click to re-lock the cursor after pointer-lock was
// released during normal gameplay (menu closed) - most commonly by the user pressing Esc.
// Browsers hard-code Esc to force-release an active pointer-lock themselves and, so pages
// can't react to that exact moment (e.g. by instantly re-trapping the cursor), they also
// suppress the Escape keydown event itself when it's what caused the release - so
// IsKeyPressed(KEY_ESCAPE) can never see it. Detecting the actual lock state via Emscripten's
// emscripten_get_pointerlock_status() instead sidesteps that entirely, and as a bonus also
// catches any other way the lock could end up released (e.g. losing it on tab-switch).
// Re-acquiring the lock only succeeds from within a genuine click event, not from code
// reacting to it later in our own frame loop - this mirrors the same issue/fix as
// BoidsParamsMenu's own AwaitingRelock state, which covers the menu-close path.
static bool awaitingRelock = false;

void UpdateDrawFrame(void)
{
    Game& game = Game::getInstance();

    // while the boids params menu is open it owns the now-visible cursor, so camera
    // look/movement and its keybinds must be frozen
    bool menuOpen = game.boidsDisplay.IsMenuOpen();

#if defined(PLATFORM_WEB)
    if (awaitingRelock)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            awaitingRelock = false;
            DisableCursor();
        }
    }
    else if (!menuOpen)
    {
        EmscriptenPointerlockChangeEvent status;
        emscripten_get_pointerlock_status(&status);
        if (!status.isActive)
        {
            awaitingRelock = true;
            EnableCursor();
        }
    }
#else
    // desktop's DisableCursor() has no browser-style forced-release, so Esc here just quits
    // (see SetExitKey(KEY_NULL) in pg_main.cpp); Esc while the menu is open closes it instead
    if (!menuOpen && IsKeyPressed(KEY_ESCAPE))
    {
        quitRequested = true;
    }
#endif

    bool cursorLocked = !menuOpen && !awaitingRelock;

    if (cursorLocked)
    {
        UpdateCamera(&game.cam, CAMERA_FREE);
    }
    if (!awaitingRelock)
    {
        game.boidsDisplay.Update(game.cam);
    }
    game.lighting.Update(game.cam);
    if (cursorLocked)
    {
        game.painting.Update(Crosshair::GetPickRay(game.cam));
    }

    if (cursorLocked && IsKeyPressed(KEY_Z)) game.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(game.cam);
            game.lighting.BeginMode();
                game.room.Draw();
                game.painting.Draw();
            game.lighting.EndMode();
            game.boidsDisplay.Draw();
        EndMode3D();

        if (cursorLocked)
        {
            Crosshair::Draw();
        }
        game.boidsDisplay.DrawTooltip();
        game.boidsDisplay.DrawMenu();
        if (cursorLocked)
        {
            game.painting.DrawTooltip();
        }

        if (awaitingRelock)
        {
            ResumePrompt::Draw();
        }

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
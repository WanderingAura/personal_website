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

// Web only: true while waiting for a click to re-lock the cursor after pointer-lock was lost
// during normal gameplay (most commonly the browser force-releasing it on Esc). Browsers also
// suppress the Escape keydown event itself when it causes that release, so IsKeyPressed(KEY_ESCAPE)
// can never see it - polling emscripten_get_pointerlock_status() instead sidesteps that, and
// also catches any other way the lock could end up released (e.g. tab-switch). Re-acquiring the
// lock only succeeds from within a genuine click event, not from code reacting to it later.
static bool awaitingRelock = false;

#if defined(PLATFORM_WEB)
static bool wasMenuOpen = false;
// while GetTime() < this, skip the pointer-lock-status check below: the browser grants a
// (re-)lock request asynchronously, so checking immediately after one (e.g. right as the
// menu closes and calls DisableCursor() itself) can see "not locked yet" and incorrectly
// flag a lost-focus state even though the lock is about to succeed fine
static double lockGraceUntil = 0.0;
static const constexpr double LOCK_GRACE_PERIOD = 0.5;
#endif

void UpdateDrawFrame(void)
{
    Game& game = Game::getInstance();

    // while the boids params menu is open it owns the now-visible cursor, so camera
    // look/movement and its keybinds must be frozen
    bool menuOpen = game.boidsDisplay.IsMenuOpen();

    // snapshotted before anything below can flip awaitingRelock false this same frame - the
    // click that resolves it must still count as "not cursor-locked yet" for this frame, or that
    // same still-true IsMouseButtonPressed() would also register as a fresh click on the
    // painting/boids base and immediately re-trigger their own click handling (e.g. reopening
    // the painting's hyperlink)
    bool wasAwaitingRelock = awaitingRelock;

#if defined(PLATFORM_WEB)
    // BoidsParamsMenu::Close() already called DisableCursor() itself when the menu just
    // closed; start the grace period here too so the status check below doesn't fire before
    // that (asynchronous) relock has had a chance to actually succeed
    if (wasMenuOpen && !menuOpen)
    {
        lockGraceUntil = GetTime() + LOCK_GRACE_PERIOD;
    }
    wasMenuOpen = menuOpen;

    if (awaitingRelock)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            awaitingRelock = false;
            DisableCursor();
            lockGraceUntil = GetTime() + LOCK_GRACE_PERIOD;
        }
    }
    else if (!menuOpen && GetTime() >= lockGraceUntil)
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

    bool cursorLocked = !menuOpen && !wasAwaitingRelock;

    if (cursorLocked)
    {
        UpdateCamera(&game.cam, CAMERA_FIRST_PERSON);
    }
    if (!wasAwaitingRelock)
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
            game.painting.DrawHighlight();
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

        DrawFPS(300, 0);

    EndDrawing();
}

bool ShouldQuit(void)
{
    return quitRequested;
}

}
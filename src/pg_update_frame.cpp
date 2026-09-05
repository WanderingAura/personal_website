#include "raylib.h"
#include "pg_game.hpp"
#include "pg_cone.hpp"
#include "pg_crosshair.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

void UpdateDrawFrame(void)
{
    Game& game = Game::getInstance();
    UpdateCamera(&game.cam, CAMERA_FREE);
    game.boidsDisplay.Update(game.cam);
    game.lighting.Update(game.cam);

    if (IsKeyPressed(KEY_Z)) game.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(game.cam);
            game.lighting.BeginMode();
                game.room.Draw();
            game.lighting.EndMode();
            game.boidsDisplay.Draw();
        EndMode3D();

        Crosshair::Draw();
        game.boidsDisplay.DrawTooltip();

        DrawRectangle( 10, 10, 320, 93, Fade(SKYBLUE, 0.5f) );
        DrawRectangleLines( 10, 10, 320, 93, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);
        DrawFPS(300, 0);

    EndDrawing();
}

}
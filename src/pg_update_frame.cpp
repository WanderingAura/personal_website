#include "raylib.h"
#include "pg_game.hpp"
#include "pg_cone.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

void UpdateDrawFrame(void)
{
    // Update Frame
    //----------------------------------------------------------------------------------
    Game& game = Game::getInstance();
    UpdateCamera(&game.cam, CAMERA_FREE);
    game.boids.UpdateBoidPositions();

    if (IsKeyPressed(KEY_Z)) game.cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
        BeginMode3D(game.cam);
            // DrawCubeWires(game.cubePos, 2.0f, 2.0f, 2.0f, MAROON);
            DrawCube(Vector3{5.0f, 5.0f, 0.0f}, 0.0f, 10.0f, 10.0f, RED);
            rlBegin(RL_TRIANGLES);
                Cone::DrawWithTriangles((Vector3){-5.0f}, Vector3UnitX, 1.0f, 5.0f, 32, RED);
                Cone::DrawWithTriangles((Vector3){-5.0f}, Vector3UnitX + Vector3UnitY, 1.0f, 5.0f, 32, BLUE);
                game.boids.DrawBoidsViaTriangles();
            rlEnd();
            DrawGrid(10, 1.0f);
        EndMode3D();

        DrawRectangle( 10, 10, 320, 93, Fade(SKYBLUE, 0.5f) );
        DrawRectangleLines( 10, 10, 320, 93, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

}
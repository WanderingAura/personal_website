#include "pg_game.hpp"
#include <cassert>
#include <raylib.h>
#include <raymath.h>
namespace PG
{

static Game* gameInstance = nullptr;

const constexpr u32 BOIDS_NUM = 200;
const constexpr BoundingBox BOIDS_BOUNDS = BoundingBox{(Vector3){-5.0f, -5.0f, -5.0f}, (Vector3){5.0f,5.0f,5.0f}};

Game& Game::getInstance()
{
    assert(gameInstance);
    return *gameInstance;
}

Game::Game() :
    // moved next to the painting on the back wall (see `painting` below)
    boidsDisplay(BOIDS_NUM, Vector3{3.0f, 0.0f, -9.0f}, 3.0f, BoidsParams{}),
    room(Vector3{0,0,0}, Vector3{24.0f, 5.0f, 24.0f}, "assets/stair_planks_diff_1k.jpg", "assets/japanese_cedar_planks_diff_1k.jpg",
         "assets/white_stucco_ao_1k.jpg", "assets/bark_willow_diff_1k.jpg", 10.0f),
    // on the back wall (z = -12), facing +Z into the room like the wall's own normal
    painting(Vector3{-3.0f, 2.5f, -12.0f}, Vector3{0.0f, 0.0f, 1.0f}, 4.0f, "assets/recollections_game.png", "Recollections - game made in 72 hours as part of a team of 6 (2 coders)\nClick to open",
        "https://benskca.itch.io/recollections")
{
}

void Game::Initialise()
{
    if (!gameInstance)
    {
        gameInstance = new Game;
    }

    gameInstance->cam.position = (Vector3){ 10.0f, 1.7f, 10.0f };
    gameInstance->cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    gameInstance->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    gameInstance->cam.fovy = 45.0f;
    gameInstance->cam.projection = CAMERA_PERSPECTIVE;

    gameInstance->boidsDisplay.Initialise();

    gameInstance->lighting.Initialise();
    gameInstance->lighting.AddLight(LightType::Point, Vector3{ 0.0f, 4.0f, 0.0f }, Vector3Zero(), WHITE);
}

void Game::Shutdown()
{
    if (gameInstance)
    {
        delete gameInstance;
    }
}

}
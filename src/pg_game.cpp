#include "pg_game.hpp"
#include <cassert>
#include <raylib.h>
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
    boids(BOIDS_NUM, BOIDS_BOUNDS)
{
}

void Game::Initialise()
{
    if (!gameInstance)
    {
        gameInstance = new Game;
    }

    // initialise camera position
    gameInstance->cam.position = (Vector3){ 10.0f, 1.0f, 10.0f };
    gameInstance->cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    gameInstance->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    gameInstance->cam.fovy = 45.0f;
    gameInstance->cam.projection = CAMERA_PERSPECTIVE;

    // initialise cube position
    gameInstance->cubePos = (Vector3){0.0f, 0.0f, 0.0f};

    gameInstance->boids.InitialiseBoids();
}

void Game::Shutdown()
{
    if (gameInstance)
    {
        delete gameInstance;
    }
}

}
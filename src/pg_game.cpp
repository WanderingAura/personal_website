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
    boidsDisplay(BOIDS_NUM, Vector3{0,0}, 5, BoidsParams{}),
    room(Vector3{0,0,0}, Vector3{24.0f, 8.0f, 24.0f}, "assets/stair_planks_diff_1k.jpg", "assets/japanese_cedar_planks_diff_1k.jpg",
         "assets/white_stucco_ao_1k.jpg", "assets/bark_willow_diff_1k.jpg", 10.0f)
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
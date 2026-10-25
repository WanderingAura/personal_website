#include "pg_game.hpp"
#include <cassert>
#include <raylib.h>
#include <raymath.h>
namespace PG
{

static Game* gameInstance = nullptr;

const constexpr u32 BOIDS_NUM = 200;
const constexpr BoundingBox BOIDS_BOUNDS = BoundingBox{(Vector3){-5.0f, -5.0f, -5.0f}, (Vector3){5.0f,5.0f,5.0f}};

const constexpr f32 ROOM_SIZE = 8.0f;
const constexpr f32 ROOM_HALF = ROOM_SIZE / 2.0f;
const constexpr f32 ROOM_HEIGHT = 5.0f;
// keeps the pyramid roof's pitch the same as the original 24-wide room with a 10-high apex
const constexpr f32 ROOM_ROOF_HEIGHT = ROOM_SIZE * (10.0f / 24.0f);
const constexpr f32 PAINTING_Y = 2.5f;

Game& Game::getInstance()
{
    assert(gameInstance);
    return *gameInstance;
}

Game::Game() :
    // centrepiece of the room, with one painting centred on each wall around it
    boidsDisplay(BOIDS_NUM, Vector3{0.0f, 0.0f, 0.0f}, 3.0f, BoidsParams{}),
    room(Vector3{0,0,0}, Vector3{ROOM_SIZE, ROOM_HEIGHT, ROOM_SIZE}, "assets/stair_planks_diff_1k.jpg", "assets/japanese_cedar_planks_diff_1k.jpg",
         "assets/white_stucco_ao_1k.jpg", "assets/bark_willow_diff_1k.jpg", ROOM_ROOF_HEIGHT),
    // on the back wall (z = -ROOM_HALF), facing +Z into the room like the wall's own normal
    painting(Vector3{0.0f, PAINTING_Y, -ROOM_HALF}, Vector3{0.0f, 0.0f, 1.0f}, 4.0f, "assets/recollections_game.png", "Recollections - game made in 72 hours as part of a team of 6 (2 coders)\nClick to open",
        "https://benskca.itch.io/recollections"),
    // on the left wall (x = -ROOM_HALF), facing +X into the room like the wall's own normal; poster
    // image is dragon_fractal.mp4's own first frame, so its aspect ratio matches the video
    dragonPainting(Vector3{-ROOM_HALF, PAINTING_Y, 0.0f}, Vector3{1.0f, 0.0f, 0.0f}, 4.0f, "assets/dragon_fractal_poster.jpg",
        "Spinning fractal renderer using OpenGL\nClick to see source code", "https://github.com/WanderingAura/fractal_gl", "assets/dragon_fractal.mp4"),
    // on the right wall (x = ROOM_HALF), facing -X into the room like the wall's own normal
    hevcPainting(Vector3{ROOM_HALF, PAINTING_Y, 0.0f}, Vector3{-1.0f, 0.0f, 0.0f}, 4.0f, "assets/h265_low_latency.png",
        "Low-latency H.265 streaming over custom protocol\nDrops stale frames, recovers with on-demand IDRs\nClick to see source code",
        "https://github.com/WanderingAura/hevc_demo"),
    // on the front wall (z = ROOM_HALF), facing -Z into the room like the wall's own normal
    brcPainting(Vector3{0.0f, PAINTING_Y, ROOM_HALF}, Vector3{0.0f, 0.0f, -1.0f}, 4.0f, "assets/1brc_profile.png",
        "One Billion Row Challenge in C++\nSIMD parsing + 16 threads: 13.8 GB in ~0.7 s\nClick to see source code",
        "https://github.com/WanderingAura/bfbrc")
{
}

void Game::Initialise()
{
    if (!gameInstance)
    {
        gameInstance = new Game;
    }

    // in a corner, clear of the walls' and boids display's collision margins, looking at the boids
    gameInstance->cam.position = (Vector3){ ROOM_HALF - 1.2f, 1.7f, ROOM_HALF - 1.2f };
    gameInstance->cam.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    gameInstance->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    gameInstance->cam.fovy = 45.0f;
    gameInstance->cam.projection = CAMERA_PERSPECTIVE;

    gameInstance->boidsDisplay.Initialise();

    gameInstance->lighting.Initialise();
    gameInstance->lighting.AddLight(LightType::Point, Vector3{ 0.0f, 4.0f, 0.0f }, Vector3Zero(), WHITE);

    gameInstance->room.RegisterCollisions(gameInstance->collisionSystem);
    gameInstance->boidsDisplay.RegisterCollisions(gameInstance->collisionSystem);
}

void Game::Shutdown()
{
    if (gameInstance)
    {
        delete gameInstance;
    }
}

}
#pragma once
#include "raylib.h"
#include "pg_boids_display.hpp"
#include "pg_room.hpp"
#include "pg_lighting.hpp"
#include "pg_painting.hpp"
#include "pg_collision.hpp"
namespace PG
{
    class Game
    {
    public:
        Camera3D cam;
        BoidsDisplay boidsDisplay;
        Room room;
        LightingSystem lighting;
        Painting painting;
        Painting dragonPainting;
        CollisionSystem collisionSystem;

        static Game& getInstance();

        static void Initialise();

        static void Shutdown();

        // delete all copy and move semantics
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(Game&&) = delete;

    private:
        Game();
    };
}
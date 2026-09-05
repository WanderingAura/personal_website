#include "raylib.h"
#include "pg_boids_display.hpp"
#include "pg_room.hpp"
#include "pg_lighting.hpp"
namespace PG
{
    class Game
    {
    public:
        Camera3D cam;
        Vector3 cubePos;
        BoidsDisplay boidsDisplay;
        Room room;
        LightingSystem lighting;
    public:

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
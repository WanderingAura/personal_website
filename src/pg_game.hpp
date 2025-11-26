#include "raylib.h"
#include "pg_boids.hpp"
namespace PG
{
    class Game
    {
    public:
        Camera3D cam;
        Vector3 cubePos;
        Boids boids;
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
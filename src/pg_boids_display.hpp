#include "pg_boids.hpp"
namespace PG
{

class BoidsDisplay
{
    BoidsDisplay() = default;
    BoidsDisplay(u32 numBoids, Vector3 position, float size);

    void Update();

    void Draw();
private:
    Boids boids;
    Vector3 pos;
    float size;
    static const Color GLASS_COLOR;
};

}
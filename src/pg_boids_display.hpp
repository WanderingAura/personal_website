#include "pg_boids.hpp"
namespace PG
{

class BoidsDisplay
{
public:
    BoidsDisplay() = default;
    BoidsDisplay(u32 numBoids, Vector3 position, float size, const BoidsParams& params);

    void Initialise();
    void Update();

    void Draw();
private:
    Boids boids;
    Vector3 pos;
    float size;
    static const Color GLASS_COLOR;
};

}
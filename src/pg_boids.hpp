#include "raylib.h"
#include <vector>
#include "pg_types.hpp"

namespace PG
{

struct BoidsParams
{
    float vicinityDist  = 2.5f;
    float tooCloseDist  = 1.2f;
    float boidSize      = 0.3f;

    float boidSizeFraction      = 0.04f;
    // the speed at which the boid steers away from the bounds
    float boundsAvoidStrength = 0.5f;

    float cohesionFactor  = 0.02f;
    float alignmentFactor = 0.02f;
    float repulsionFactor = 0.025f;

    float speed = 30.0f;
};

class Boids
{
public:
    Boids(u32 numBoids, BoundingBox bounds, const BoidsParams& params);

    Boids() = default;

    void Initialise(u32 numBoids, BoundingBox bounds, const BoidsParams& params);

    void InitialiseBoids();

    void UpdateBoidPositions();

    // this should be called within rlBegin(RL_TRIANGLES)
    void DrawBoidsViaTriangles();
private:

    bool isInitialised = false;

    BoidsParams params;

    static const constexpr Vector3 BOUNDS_MARGIN = {0.5f, 0.5f, 0.5f};
    std::vector<Vector3> positions;
    std::vector<Vector3> dirs;
public:
    BoundingBox bounds;
};

}
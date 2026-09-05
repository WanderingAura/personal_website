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

    // How strongly a boid steers away from the bounds once inside boundsAvoidMargin of an
    // edge, as a 0..1 blend factor applied right at the edge (ramps up from 0 as the boid
    // gets closer). 1 means the direction is fully turned around by the time it reaches the edge.
    float boundsAvoidStrength = 0.5f;

    float cohesionFactor  = 0.01f;
    float alignmentFactor = 0.01f;
    float repulsionFactor = 0.025f;

    float speed = 45.0f;
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
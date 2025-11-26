#include "raylib.h"
#include <vector>
#include "pg_types.hpp"

namespace PG
{

struct BoidParams
{
    float vicinityDist;
    float tooCloseDist;
    float boidSize;
    float turnSpeed;

    float cohesionFactor;
    float alignmentFactor;
    float repulsionFactor;

    float speed;
};

class Boids
{
public:
    Boids(u32 numBoids, BoundingBox bounds);

    Boids() = default;

    void Initialise(u32 numBoids, BoundingBox bounds);

    void InitialiseBoids();

    void UpdateBoidPositions();

    // this should be called within rlBegin(RL_TRIANGLES)
    void DrawBoidsViaTriangles();
private:

    bool isInitialised = false;

    BoidParams params;

    static const constexpr Vector3 BOUNDS_MARGIN = {0.5f, 0.5f, 0.5f};
    std::vector<Vector3> positions;
    std::vector<Vector3> dirs;
public:
    BoundingBox bounds;
};

}
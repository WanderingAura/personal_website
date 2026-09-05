#include "pg_boids_display.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
namespace PG
{

BoidsDisplay::BoidsDisplay(u32 numBoids, Vector3 position, float size, const BoidsParams& params) :
    pos(position),
    size(size)
{
    // calculate the bounding box from size and position
    Vector3 minPoint = Vector3SubtractValue(position, size / 2);
    Vector3 maxPoint = Vector3AddValue(position, size / 2);
    BoundingBox boidsBoundingBox = {minPoint, maxPoint};

    boids.Initialise(numBoids, boidsBoundingBox, params);
}

void BoidsDisplay::Initialise()
{
    boids.InitialiseBoids();
}

void BoidsDisplay::Update()
{
    boids.UpdateBoidPositions();
}

using Dimensions = Vector3;

static inline Dimensions GetDimsFromBoundingBox(BoundingBox box)
{
    return Vector3Subtract(box.max, box.min);
}

const Color BoidsDisplay::GLASS_COLOR = {0xc7, 0xfe, 0xff, 128};

void BoidsDisplay::Draw()
{
    rlBegin(RL_TRIANGLES);
        boids.DrawBoidsViaTriangles();
    rlEnd();

    Dimensions dims = GetDimsFromBoundingBox(boids.bounds);

    DrawCube(boids.bounds.min, dims.x, dims.y, dims.z, GLASS_COLOR);
}

}
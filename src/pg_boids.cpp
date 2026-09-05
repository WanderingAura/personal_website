#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include "pg_boids.hpp"
#include "pg_cone.hpp"

namespace PG
{

static float SteerAxisFromBounds(float dirComponent, float pos, float minBound, float maxBound, float margin, float boundsAvoidStrength)
{
    float distFromMin = pos - minBound;
    if (distFromMin < margin)
    {
        float depth = 1.0f - Clamp(distFromMin / margin, 0.0f, 1.0f);
        return Lerp(dirComponent, fabsf(dirComponent), boundsAvoidStrength * depth * depth);
    }

    float distFromMax = maxBound - pos;
    if (distFromMax < margin)
    {
        float depth = 1.0f - Clamp(distFromMax / margin, 0.0f, 1.0f);
        return Lerp(dirComponent, -fabsf(dirComponent), boundsAvoidStrength * depth * depth);
    }

    return dirComponent;
}

Boids::Boids(u32 numBoids, BoundingBox bounds, const BoidsParams& params)
{
    Initialise(numBoids, bounds, params);
}

void Boids::Initialise(u32 numBoids, BoundingBox bounds, const BoidsParams& params)
{
    this->bounds = bounds;
    this->params = params;
    positions.resize(numBoids);
    dirs.resize(numBoids);
}

void Boids::InitialiseBoids()
{
    assert(dirs.size() == positions.size());
    assert(positions.size() > 0);

    Vector3 minRand = Vector3Add(bounds.min, BOUNDS_MARGIN);
    Vector3 maxRand = Vector3Subtract(bounds.max, BOUNDS_MARGIN);

    for (u32 i = 0; i < positions.size(); i++)
    {
        positions[i] = (Vector3)
        {
            (float)GetRandomValue(minRand.x, maxRand.x),
            (float)GetRandomValue(minRand.y, maxRand.y),
            (float)GetRandomValue(minRand.z, maxRand.z),
        };

        // TODO: probably should check if this actually generates uniform spherical coords
        float yRotate = (float)GetRandomValue(0, 359);
        float zRotate = (float)GetRandomValue(-90, 90);
        Vector3 rotated = Vector3RotateByAxisAngle(Vector3UnitX, Vector3UnitY, yRotate);
        dirs[i] = Vector3RotateByAxisAngle(rotated, Vector3UnitZ, zRotate);
    }

    isInitialised = true;
}

void Boids::UpdateBoidPositions()
{
    // TODO: we are currently using the naive O(n^2) approach to collision
    // possibly look into splitting space into regions and only checking for collisions in nearby regions

    assert(isInitialised);
    assert(dirs.size() == positions.size());
    assert(positions.size() > 0);

    for (u64 i = 0; i < positions.size(); i++)
    {
        Vector3 curBoidPos = positions[i];
        u32 numInVicinity = 0;
        u32 numTooClose = 0;
        Vector3 centreOfMass = {};
        Vector3 meanRepulseDir = {};
        Vector3 meanDir = {};

        // accumulate centre of mass, mean direction, and mean repulsion direction of nearby boids
        for (u64 j = 0; j < positions.size() && i != j; j++)
        {
            Vector3 otherBoidPos = positions[j];
            Vector3 otherBoidDir = dirs[j];

            bool isInVicinity = CheckCollisionSpheres(curBoidPos, params.vicinityDist, otherBoidPos, params.boidSize);
            if (isInVicinity)
            {
                numInVicinity += 1;
                centreOfMass = Vector3Add(centreOfMass, otherBoidPos);
                meanDir = Vector3Add(meanDir, otherBoidDir);
            }

            // TODO: could merge with the vicinity check above into one custom collision test
            bool isTooClose = CheckCollisionSpheres(curBoidPos, params.tooCloseDist, otherBoidPos, params.boidSize);
            if (isTooClose)
            {
                numTooClose += 1;
                Vector3 repulseDir = Vector3Subtract(curBoidPos, otherBoidPos);
                // invert the magnitude of the repulsion vector. so the closer boids repulse more
                if (repulseDir != Vector3Zero())
                {
                    repulseDir = Vector3Scale(repulseDir, 1.0f / Vector3LengthSqr(repulseDir));
                }
                meanRepulseDir = Vector3Add(meanRepulseDir, repulseDir);
            }
        }

        Vector3 towardsCenterOfMass = {};
        if (numInVicinity > 0)
        {
            centreOfMass = Vector3Scale(centreOfMass, 1.0f/numInVicinity);
            towardsCenterOfMass = Vector3Subtract(centreOfMass, curBoidPos);

            // TODO: further boids pull harder towards the flock (unnormalised) - is that right?
            towardsCenterOfMass = Vector3Scale(towardsCenterOfMass, params.cohesionFactor);

            meanDir = Vector3Scale(meanDir, params.alignmentFactor / numInVicinity);
        }

        if (numTooClose > 0)
        {
            meanRepulseDir = Vector3Scale(meanRepulseDir, params.repulsionFactor / numTooClose);
        }

        dirs[i] = Vector3Add(dirs[i], Vector3Add(towardsCenterOfMass, Vector3Add(meanDir, meanRepulseDir)));

        dirs[i].x = SteerAxisFromBounds(dirs[i].x, curBoidPos.x, bounds.min.x, bounds.max.x, BOUNDS_MARGIN.x, params.boundsAvoidStrength);
        dirs[i].y = SteerAxisFromBounds(dirs[i].y, curBoidPos.y, bounds.min.y, bounds.max.y, BOUNDS_MARGIN.y, params.boundsAvoidStrength);
        dirs[i].z = SteerAxisFromBounds(dirs[i].z, curBoidPos.z, bounds.min.z, bounds.max.z, BOUNDS_MARGIN.z, params.boundsAvoidStrength);

        dirs[i] = Vector3ClampValue(dirs[i], 0.02f, 0.2f);

        float dt = GetFrameTime();
        // clamp dt for the web build
        if (dt > 0.1f)
        {
            dt = 0.1f;
        }
        Vector3 step = Vector3Scale(dirs[i], dt*params.speed);
        positions[i] = Vector3Add(positions[i], step);
    }
}

void Boids::DrawBoidsViaTriangles()
{
    for (u32 i = 0; i < positions.size(); i++)
    {
        Cone::DrawWithTriangles(positions[i], dirs[i], params.boidSize * 0.5f, params.boidSize, 16, PINK);
    }
}

BoidsParams& Boids::GetParams()
{
    return params;
}

}

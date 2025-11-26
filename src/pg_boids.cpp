#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include "pg_boids.hpp"
#include "pg_cone.hpp"

namespace PG
{

const constexpr float DEFAULT_VICINITY = 2.5f;
const constexpr float DEFAULT_CLOSE = 1.2f;
const constexpr float DEFAULT_BOID_SIZE = 0.3f;
const constexpr float DEFAULT_ALIGNMENT_FACTOR = 0.01f;
const constexpr float DEFAULT_COHESION_FACTOR = 0.01f;
const constexpr float DEFAULT_REPULSION_FACTOR = 0.025f;
const constexpr float DEFAULT_TURN_SPEED = 0.01f;
const constexpr float DEFAULT_SPEED = 45.0f;



Boids::Boids(u32 numBoids, BoundingBox bounds) :
    bounds(bounds)
{
    positions.resize(numBoids);
    dirs.resize(numBoids);
}

void Boids::Initialise(u32 numBoids, BoundingBox b)
{
    positions.resize(numBoids);
    dirs.resize(numBoids);
    bounds = b;
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

    params.vicinityDist = DEFAULT_VICINITY;
    params.tooCloseDist = DEFAULT_CLOSE;
    params.boidSize = DEFAULT_BOID_SIZE;
    
    params.alignmentFactor = DEFAULT_ALIGNMENT_FACTOR;
    params.cohesionFactor = DEFAULT_COHESION_FACTOR;
    params.repulsionFactor = DEFAULT_REPULSION_FACTOR;
    params.turnSpeed = DEFAULT_TURN_SPEED;
    params.speed = DEFAULT_SPEED;

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

        // Calculate 3 things:
        // 1. centre of mass of nearby birds
        // 2. mean direction of nearby birds
        // 3. mean direction to go to avoid birds that are too close
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

            // NOTE: can probably make a custom collision checker that does both collision checks
            // at the same time.
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

            // NOTE: the further away a boid is, the more it wants to be closer to the flock.
            // is this actually how flocks work? or should i normalise this vector?
            towardsCenterOfMass = Vector3Scale(towardsCenterOfMass, params.cohesionFactor);

            meanDir = Vector3Scale(meanDir, params.alignmentFactor / numInVicinity);
        }

        if (numTooClose > 0)
        {
            meanRepulseDir = Vector3Scale(meanRepulseDir, params.repulsionFactor / numTooClose);
        }

        dirs[i] = Vector3Add(dirs[i], Vector3Add(towardsCenterOfMass, Vector3Add(meanDir, meanRepulseDir)));
        
        // Adjust to avoid bounds
        // ----------------------------

        // TODO: there must be a better way to do this with less branches
        // could check disassembly to check if compiler optimises well.
        if (curBoidPos.x < bounds.min.x + BOUNDS_MARGIN.x)
        {
            dirs[i].x += params.turnSpeed;
        }
        if (curBoidPos.x > bounds.max.x - BOUNDS_MARGIN.x)
        {
            dirs[i].x -= params.turnSpeed;
        }
        if (curBoidPos.y < bounds.min.y + BOUNDS_MARGIN.y)
        {
            dirs[i].y += params.turnSpeed;
        }
        if (curBoidPos.y > bounds.max.y - BOUNDS_MARGIN.y)
        {
            dirs[i].y -= params.turnSpeed;
        }
        if (curBoidPos.z < bounds.min.z + BOUNDS_MARGIN.z)
        {
            dirs[i].z += params.turnSpeed;
        }
        if (curBoidPos.z > bounds.max.z - BOUNDS_MARGIN.z)
        {
            dirs[i].z -= params.turnSpeed;
        }

        dirs[i] = Vector3ClampValue(dirs[i], 0.02f, 0.2f);

        // Move the boid
        // ----------------
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

}
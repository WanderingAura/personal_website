#pragma once
#include "pg_types.hpp"
#include <raylib.h>

namespace PG
{

// An axis-aligned rectangle in the XZ (horizontal) plane. Y/height is ignored everywhere in
// this system - the player can't jump, so collision only needs to be 2D.
struct CollisionRect
{
    float minX, maxX, minZ, maxZ;
};

// A basic keep-out collision system: objects register the rectangles they occupy (e.g. via
// their own RegisterCollisions() method, see Room/BoidsDisplay), and the camera is pushed back
// out whenever it ends up within COLLISION_MARGIN of any of them. A wall registered as a thin
// rectangle along its own plane works the same way as a solid object - the margin keeps the
// player from ever reaching the rectangle itself, so they can't cross through it either.
class CollisionSystem
{
public:
    static const constexpr float COLLISION_MARGIN = 0.6f;

    // registers a rectangle the camera should be kept COLLISION_MARGIN away from
    void RegisterCollidable(CollisionRect rect);

    // pushes position.x/position.z outward if they've ended up within COLLISION_MARGIN of any
    // registered rectangle; call every frame after moving the camera
    void ResolvePosition(Vector3& position) const;

private:
    static const constexpr u32 MAX_COLLIDABLES = 32;
    CollisionRect collidables[MAX_COLLIDABLES];
    u32 count = 0;
};

}

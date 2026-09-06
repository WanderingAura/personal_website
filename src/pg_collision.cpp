#include "pg_collision.hpp"
#include <algorithm>

namespace PG
{

void CollisionSystem::RegisterCollidable(CollisionRect rect)
{
    if (count >= MAX_COLLIDABLES) return;
    collidables[count++] = rect;
}

void CollisionSystem::ResolvePosition(Vector3& position) const
{
    for (u32 i = 0; i < count; i++)
    {
        const CollisionRect& rect = collidables[i];
        float minX = rect.minX - COLLISION_MARGIN;
        float maxX = rect.maxX + COLLISION_MARGIN;
        float minZ = rect.minZ - COLLISION_MARGIN;
        float maxZ = rect.maxZ + COLLISION_MARGIN;

        if (position.x <= minX || position.x >= maxX || position.z <= minZ || position.z >= maxZ)
        {
            continue;
        }

        // inside the margin - push out along whichever edge is nearest
        float distToMinX = position.x - minX;
        float distToMaxX = maxX - position.x;
        float distToMinZ = position.z - minZ;
        float distToMaxZ = maxZ - position.z;
        float minDist = std::min({ distToMinX, distToMaxX, distToMinZ, distToMaxZ });

        if (minDist == distToMinX) position.x = minX;
        else if (minDist == distToMaxX) position.x = maxX;
        else if (minDist == distToMinZ) position.z = minZ;
        else position.z = maxZ;
    }
}

}

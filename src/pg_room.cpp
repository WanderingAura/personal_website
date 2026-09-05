#include "pg_room.hpp"
#include <rlgl.h>

namespace PG
{

// world units per texture repeat, so a 4k detail texture doesn't stretch across a whole wall
static const constexpr float TEXTURE_TILE_SIZE = 2.0f;

static Texture2D LoadTiledTexture(const char* path)
{
    Texture2D texture = LoadTexture(path);
    SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);
    // LoadTexture alone has no mipmaps, so distant/tilted walls alias badly; generate them
    // and switch to trilinear filtering so the GPU actually picks a lower-res mip up close
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    return texture;
}

Room::Room(Vector3 position, Vector3 size, const char* wallTexturePath, const char* floorTexturePath) :
    position(position),
    size(size),
    wallTexture(LoadTiledTexture(wallTexturePath)),
    floorTexture(LoadTiledTexture(floorTexturePath))
{
}

// emits one quad into the active RL_QUADS batch; corners must be wound so the desired normal
// points towards the viewer (i.e. into the room)
static void DrawTexturedQuad(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 normal, float uMax, float vMax)
{
    rlNormal3f(normal.x, normal.y, normal.z);

    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(v0.x, v0.y, v0.z);
    rlTexCoord2f(0.0f, vMax); rlVertex3f(v1.x, v1.y, v1.z);
    rlTexCoord2f(uMax, vMax); rlVertex3f(v2.x, v2.y, v2.z);
    rlTexCoord2f(uMax, 0.0f); rlVertex3f(v3.x, v3.y, v3.z);
}

void Room::Draw() const
{
    float minX = position.x - size.x / 2;
    float maxX = position.x + size.x / 2;
    float minZ = position.z - size.z / 2;
    float maxZ = position.z + size.z / 2;
    float floorY = position.y;
    float ceilY = position.y + size.y;

    rlColor4ub(255, 255, 255, 255);

    rlSetTexture(floorTexture.id);
    rlBegin(RL_QUADS);
        DrawTexturedQuad(
            Vector3{ minX, floorY, minZ }, Vector3{ minX, floorY, maxZ },
            Vector3{ maxX, floorY, maxZ }, Vector3{ maxX, floorY, minZ },
            Vector3{ 0.0f, 1.0f, 0.0f }, size.x / TEXTURE_TILE_SIZE, size.z / TEXTURE_TILE_SIZE);
    rlEnd();
    rlSetTexture(0);

    rlSetTexture(wallTexture.id);
    rlBegin(RL_QUADS);
        // back wall (minZ), facing +Z into the room
        DrawTexturedQuad(
            Vector3{ minX, floorY, minZ }, Vector3{ maxX, floorY, minZ },
            Vector3{ maxX, ceilY, minZ }, Vector3{ minX, ceilY, minZ },
            Vector3{ 0.0f, 0.0f, 1.0f }, size.x / TEXTURE_TILE_SIZE, size.y / TEXTURE_TILE_SIZE);

        // front wall (maxZ), facing -Z into the room
        DrawTexturedQuad(
            Vector3{ minX, floorY, maxZ }, Vector3{ minX, ceilY, maxZ },
            Vector3{ maxX, ceilY, maxZ }, Vector3{ maxX, floorY, maxZ },
            Vector3{ 0.0f, 0.0f, -1.0f }, size.x / TEXTURE_TILE_SIZE, size.y / TEXTURE_TILE_SIZE);

        // left wall (minX), facing +X into the room
        DrawTexturedQuad(
            Vector3{ minX, floorY, minZ }, Vector3{ minX, ceilY, minZ },
            Vector3{ minX, ceilY, maxZ }, Vector3{ minX, floorY, maxZ },
            Vector3{ 1.0f, 0.0f, 0.0f }, size.z / TEXTURE_TILE_SIZE, size.y / TEXTURE_TILE_SIZE);

        // right wall (maxX), facing -X into the room
        DrawTexturedQuad(
            Vector3{ maxX, floorY, minZ }, Vector3{ maxX, floorY, maxZ },
            Vector3{ maxX, ceilY, maxZ }, Vector3{ maxX, ceilY, minZ },
            Vector3{ -1.0f, 0.0f, 0.0f }, size.z / TEXTURE_TILE_SIZE, size.y / TEXTURE_TILE_SIZE);
    rlEnd();
    rlSetTexture(0);
}

}

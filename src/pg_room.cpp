#include "pg_room.hpp"
#include "pg_types.hpp"
#include "pg_vector_utils.hpp"
#include <rlgl.h>
#include <raymath.h>

namespace PG
{

// world units per texture repeat, so a 4k detail texture doesn't stretch across a whole wall
static const constexpr float TEXTURE_TILE_SIZE = 2.0f;

// beams are thin, so tile their wood grain more often than a wall would
static const constexpr float BEAM_TILE_SIZE = 1.0f;
static const constexpr float HIP_BEAM_THICKNESS = 0.35f;
static const constexpr float EAVE_BEAM_THICKNESS = 0.35f;
static const constexpr float RAFTER_BEAM_THICKNESS = 0.2f;
// number of equal divisions each roof edge is split into by rafters (so RAFTER_DIVISIONS - 1
// intermediate rafters per face, in addition to the hip beam at each corner)
static const constexpr u32 RAFTER_DIVISIONS = 4;

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

Room::Room(Vector3 position, Vector3 size, const char* wallTexturePath, const char* floorTexturePath,
           const char* roofTexturePath, const char* beamTexturePath, float roofHeight) :
    position(position),
    size(size),
    roofHeight(roofHeight),
    wallTexture(LoadTiledTexture(wallTexturePath)),
    floorTexture(LoadTiledTexture(floorTexturePath)),
    roofTexture(LoadTiledTexture(roofTexturePath)),
    beamTexture(LoadTiledTexture(beamTexturePath))
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

// emits one triangle into the active RL_TRIANGLES batch, texcoords given directly per vertex
// since a roof face isn't a rectangle
static void DrawTexturedTriangle(Vector3 v0, Vector3 v1, Vector3 v2, Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector3 normal)
{
    rlNormal3f(normal.x, normal.y, normal.z);

    rlTexCoord2f(uv0.x, uv0.y); rlVertex3f(v0.x, v0.y, v0.z);
    rlTexCoord2f(uv1.x, uv1.y); rlVertex3f(v1.x, v1.y, v1.z);
    rlTexCoord2f(uv2.x, uv2.y); rlVertex3f(v2.x, v2.y, v2.z);
}

// emits a thin rectangular beam spanning start->end into the active RL_QUADS batch, textured
// so the grain runs along its length (tiled every BEAM_TILE_SIZE world units)
static void DrawTexturedBeam(Vector3 start, Vector3 end, float thickness)
{
    Vector3 dir = Vector3Normalize(Vector3Subtract(end, start));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(dir, NonParallelReference(dir)));
    Vector3 up = Vector3CrossProduct(right, dir);

    float half = thickness * 0.5f;
    Vector3 offsets[4] = {
        Vector3Add(Vector3Scale(right, -half), Vector3Scale(up, -half)),
        Vector3Add(Vector3Scale(right,  half), Vector3Scale(up, -half)),
        Vector3Add(Vector3Scale(right,  half), Vector3Scale(up,  half)),
        Vector3Add(Vector3Scale(right, -half), Vector3Scale(up,  half)),
    };

    float vMax = Vector3Distance(start, end) / BEAM_TILE_SIZE;

    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;
        Vector3 normal = Vector3Normalize(Vector3Add(offsets[i], offsets[j]));
        DrawTexturedQuad(
            Vector3Add(start, offsets[i]), Vector3Add(end, offsets[i]),
            Vector3Add(end, offsets[j]), Vector3Add(start, offsets[j]),
            normal, 1.0f, vMax);
    }
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
            Vector3{ maxX, floorY, minZ }, Vector3{ maxX, ceilY, minZ },
            Vector3{ minX, ceilY, minZ }, Vector3{ minX, floorY, minZ },
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
            Vector3{ maxX, ceilY, minZ }, Vector3{ maxX, floorY, minZ },
            Vector3{ maxX, floorY, maxZ }, Vector3{ maxX, ceilY, maxZ },
            Vector3{ -1.0f, 0.0f, 0.0f }, size.z / TEXTURE_TILE_SIZE, size.y / TEXTURE_TILE_SIZE);
    rlEnd();
    rlSetTexture(0);

    // square-based pyramid roof sitting on top of the walls, apex centred above the room
    Vector3 apex = { position.x, ceilY + roofHeight, position.z };
    Vector3 corners[4] = {
        Vector3{ minX, ceilY, minZ }, Vector3{ minX, ceilY, maxZ },
        Vector3{ maxX, ceilY, maxZ }, Vector3{ maxX, ceilY, minZ },
    };

    rlSetTexture(roofTexture.id);
    rlBegin(RL_TRIANGLES);
        for (int i = 0; i < 4; i++)
        {
            Vector3 c0 = corners[i];
            Vector3 c1 = corners[(i + 1) % 4];
            float baseLen = Vector3Distance(c0, c1);
            float slantLen = Vector3Distance(c0, apex);
            float uMax = baseLen / TEXTURE_TILE_SIZE;
            float vMax = slantLen / TEXTURE_TILE_SIZE;
            Vector3 normal = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(c1, c0), Vector3Subtract(apex, c1)));
            DrawTexturedTriangle(c0, c1, apex, Vector2{ 0.0f, 0.0f }, Vector2{ uMax, 0.0f }, Vector2{ uMax * 0.5f, vMax }, normal);
        }
    rlEnd();
    rlSetTexture(0);

    rlSetTexture(beamTexture.id);
    rlBegin(RL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            Vector3 c0 = corners[i];
            Vector3 c1 = corners[(i + 1) % 4];

            // hip beam: corner up to the apex
            DrawTexturedBeam(c0, apex, HIP_BEAM_THICKNESS);
            // eave beam: along the base edge between corners
            DrawTexturedBeam(c0, c1, EAVE_BEAM_THICKNESS);

            // rafters: evenly spaced beams that rise perpendicular to the base edge (i.e.
            // "vertical" within the sloped face, like real common rafters) rather than toward
            // the apex; each stops wherever that line meets whichever hip beam is on its side
            // (the middle one, equidistant from both hips, meets the apex itself)
            Vector3 mid = Vector3Lerp(c0, c1, 0.5f);
            Vector3 upSlope = Vector3Normalize(Vector3Subtract(apex, mid));
            float slantLen = Vector3Distance(mid, apex);

            for (u32 k = 1; k < RAFTER_DIVISIONS; k++)
            {
                float t = (float)k / (float)RAFTER_DIVISIONS;
                Vector3 basePoint = Vector3Lerp(c0, c1, t);
                float rafterLen = slantLen * (1.0f - 2.0f * fabsf(t - 0.5f));
                Vector3 top = Vector3Add(basePoint, Vector3Scale(upSlope, rafterLen));
                DrawTexturedBeam(basePoint, top, RAFTER_BEAM_THICKNESS);
            }
        }
    rlEnd();
    rlSetTexture(0);
}

}

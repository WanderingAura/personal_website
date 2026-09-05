#include "pg_highlight_edge.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

static void DrawSolidQuad(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 normal)
{
    rlNormal3f(normal.x, normal.y, normal.z);
    rlVertex3f(v0.x, v0.y, v0.z);
    rlVertex3f(v1.x, v1.y, v1.z);
    rlVertex3f(v2.x, v2.y, v2.z);
    rlVertex3f(v3.x, v3.y, v3.z);
}

void DrawHighlightEdge(Vector3 start, Vector3 end, float thickness, Color color)
{
    Vector3 dir = Vector3Normalize(Vector3Subtract(end, start));
    // arbitrary reference perpendicular-ish to dir, just needs to not be parallel to it
    Vector3 reference = (fabsf(dir.y) > 0.99f) ? Vector3{ 1.0f, 0.0f, 0.0f } : Vector3{ 0.0f, 1.0f, 0.0f };
    Vector3 right = Vector3Normalize(Vector3CrossProduct(dir, reference));
    Vector3 up = Vector3CrossProduct(right, dir);

    float half = thickness * 0.5f;
    Vector3 offsets[4] = {
        Vector3Add(Vector3Scale(right, -half), Vector3Scale(up, -half)),
        Vector3Add(Vector3Scale(right,  half), Vector3Scale(up, -half)),
        Vector3Add(Vector3Scale(right,  half), Vector3Scale(up,  half)),
        Vector3Add(Vector3Scale(right, -half), Vector3Scale(up,  half)),
    };

    rlColor4ub(color.r, color.g, color.b, color.a);
    rlBegin(RL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            int j = (i + 1) % 4;
            Vector3 normal = Vector3Normalize(Vector3Add(offsets[i], offsets[j]));
            DrawSolidQuad(
                Vector3Add(start, offsets[i]), Vector3Add(end, offsets[i]),
                Vector3Add(end, offsets[j]), Vector3Add(start, offsets[j]),
                normal);
        }
    rlEnd();

    // round off the joints where edges meet - DrawSphere (unlike DrawCylinderEx above) does
    // set its own normals correctly, so it's safe to use as-is
    float radius = thickness * 0.5f;
    DrawSphere(start, radius, color);
    DrawSphere(end, radius, color);
}

}

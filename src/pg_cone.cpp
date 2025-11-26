#include "pg_cone.hpp"
#include "raymath.h"
#include "rlgl.h"
#include <raylib.h>

namespace PG
{

void Cone::DrawWithTriangles(Vector3 pos, Vector3 dir, float radius, float height, u32 slices, Color color)
{
    float sliceAngle = 2*PI / slices;

    dir = Vector3Normalize(dir);
    Vector3 halfHeight = Vector3Scale(dir, height/2);
    Vector3 coneTop = Vector3Add(pos, halfHeight);

    Vector3 coneBaseCenter = Vector3Subtract(pos, halfHeight);

    Vector3 radiusDir = Vector3Scale(Vector3Perpendicular(dir), radius);

    rlColor4ub(color.r, color.g, color.b, color.a);

    for (u32 i = 0; i < slices; i++)
    {
        float angle1 = i * sliceAngle;
        float angle2 = (i + 1) * sliceAngle;

        Vector3 baseDir1 = Vector3RotateByAxisAngle(radiusDir, dir, angle1);
        Vector3 baseDir2 = Vector3RotateByAxisAngle(radiusDir, dir, angle2);
        Vector3 base1 = Vector3Add(coneBaseCenter, baseDir1);
        Vector3 base2 = Vector3Add(coneBaseCenter, baseDir2);

        Color baseColor = Color{(u8)(color.r/2), (u8)(color.g/2), (u8)(color.b/2), color.a};
        rlColor4ub(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
        rlVertex3f(base1.x, base1.y, base1.z);
        rlVertex3f(coneBaseCenter.x, coneBaseCenter.y, coneBaseCenter.z);
        rlVertex3f(base2.x, base2.y, base2.z);

        rlColor4ub(0, 0, 0, 255);
        rlVertex3f(coneTop.x, coneTop.y, coneTop.z);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(base1.x, base1.y, base1.z);
        rlVertex3f(base2.x, base2.y, base2.z);
    }
}

}
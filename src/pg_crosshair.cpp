#include "pg_crosshair.hpp"
#include <raylib.h>

namespace PG
{

static const constexpr int ARM_LENGTH = 10;
static const constexpr int GAP = 4;
static const constexpr int THICKNESS = 2;

void Crosshair::Draw()
{
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;

    // four separate arms, leaving a small gap in the middle so it doesn't obscure what's behind it
    DrawRectangle(centerX - THICKNESS / 2, centerY - GAP - ARM_LENGTH, THICKNESS, ARM_LENGTH, WHITE);
    DrawRectangle(centerX - THICKNESS / 2, centerY + GAP, THICKNESS, ARM_LENGTH, WHITE);
    DrawRectangle(centerX - GAP - ARM_LENGTH, centerY - THICKNESS / 2, ARM_LENGTH, THICKNESS, WHITE);
    DrawRectangle(centerX + GAP, centerY - THICKNESS / 2, ARM_LENGTH, THICKNESS, WHITE);
}

Ray Crosshair::GetPickRay(Camera3D cam)
{
    Vector2 screenCenter = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    return GetScreenToWorldRay(screenCenter, cam);
}

}

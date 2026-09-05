#include "pg_hoverable_box.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

static const constexpr float HOVER_OUTLINE_THICKNESS = 4.0f;

HoverableBox::HoverableBox(BoundingBox bounds, Color color, const char* tooltipText) :
    bounds(bounds),
    color(color),
    tooltipText(tooltipText)
{
}

void HoverableBox::Update(Ray ray)
{
    RayCollision collision = GetRayCollisionBox(ray, bounds);
    isHovered = collision.hit;
}

void HoverableBox::Draw() const
{
    Vector3 dims = Vector3Subtract(bounds.max, bounds.min);
    Vector3 center = Vector3Add(bounds.min, Vector3Scale(dims, 0.5f));
    DrawCube(center, dims.x, dims.y, dims.z, color);

    if (isHovered)
    {
        rlSetLineWidth(HOVER_OUTLINE_THICKNESS);
        DrawCubeWires(center, dims.x, dims.y, dims.z, WHITE);
        rlSetLineWidth(1.0f);
    }
}

void HoverableBox::DrawTooltip() const
{
    if (isHovered)
    {
        Tooltip::DrawAtBottom(tooltipText);
    }
}

bool HoverableBox::IsHovered() const
{
    return isHovered;
}

}

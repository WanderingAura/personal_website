#include "pg_hoverable_box.hpp"
#include "pg_highlight_edge.hpp"
#include <raymath.h>

namespace PG
{

// world-space thickness of the hover outline edges. GL line width (rlSetLineWidth) can't be used
// for this - most desktop core-profile drivers and WebGL clamp it to 1px regardless of the
// requested value - so the outline is instead built from real geometry (see DrawHighlightEdge).
static const constexpr float HOVER_OUTLINE_THICKNESS = 0.06f;

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
        Vector3 min = bounds.min;
        Vector3 max = bounds.max;
        Vector3 c000 = { min.x, min.y, min.z };
        Vector3 c001 = { min.x, min.y, max.z };
        Vector3 c010 = { min.x, max.y, min.z };
        Vector3 c011 = { min.x, max.y, max.z };
        Vector3 c100 = { max.x, min.y, min.z };
        Vector3 c101 = { max.x, min.y, max.z };
        Vector3 c110 = { max.x, max.y, min.z };
        Vector3 c111 = { max.x, max.y, max.z };

        // bottom face
        DrawHighlightEdge(c000, c100, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c100, c101, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c101, c001, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c001, c000, HOVER_OUTLINE_THICKNESS, WHITE);
        // top face
        DrawHighlightEdge(c010, c110, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c110, c111, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c111, c011, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c011, c010, HOVER_OUTLINE_THICKNESS, WHITE);
        // verticals
        DrawHighlightEdge(c000, c010, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c100, c110, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c101, c111, HOVER_OUTLINE_THICKNESS, WHITE);
        DrawHighlightEdge(c001, c011, HOVER_OUTLINE_THICKNESS, WHITE);
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

#include "raylib.h"
#include "pg_tooltip.hpp"

namespace PG
{

// A solid 3D box that shows a tooltip at the bottom of the screen while the mouse hovers over it.
class HoverableBox
{
public:
    HoverableBox() = default;
    HoverableBox(BoundingBox bounds, Color color, const char* tooltipText);

    // must be called every frame with the current pick ray (e.g. Crosshair::GetPickRay) to
    // keep the hover state up to date
    void Update(Ray ray);

    // draws the solid box; must be called within BeginMode3D/EndMode3D
    void Draw() const;

    // draws the tooltip if currently hovered; must be called in 2D (outside of BeginMode3D/EndMode3D)
    void DrawTooltip() const;

    bool IsHovered() const;

private:
    BoundingBox bounds;
    Color color;
    const char* tooltipText;
    bool isHovered = false;
};

}

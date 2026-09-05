#include "pg_boids_display.hpp"
#include "pg_crosshair.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
namespace PG
{

static const constexpr float BASE_THICKNESS_FRACTION = 0.3f;

BoidsDisplay::BoidsDisplay(u32 numBoids, Vector3 position, float size, const BoidsParams& params) :
    pos(position),
    size(size)
{
    // position is the centre of the bottom face of the base; the glass case sits directly
    // on top of it, sharing the same footprint
    float baseThickness = size * BASE_THICKNESS_FRACTION;
    BoundingBox baseBounds =
    {
        Vector3{ position.x - size / 2, position.y, position.z - size / 2 },
        Vector3{ position.x + size / 2, position.y + baseThickness, position.z + size / 2 },
    };
    base = HoverableBox(baseBounds, BASE_COLOR, "Boids flocking simulation");
    baseLabel = BoxLabel(baseBounds, "Click here to adjust parameters");

    BoundingBox boidsBoundingBox =
    {
        Vector3{ position.x - size / 2, baseBounds.max.y, position.z - size / 2 },
        Vector3{ position.x + size / 2, baseBounds.max.y + size, position.z + size / 2 },
    };

    // scale boid size with the display's own size, rather than using an absolute size
    // that stays fixed regardless of how big the box is
    BoidsParams scaledParams = params;
    scaledParams.boidSize = size * params.boidSizeFraction;

    boids.Initialise(numBoids, boidsBoundingBox, scaledParams);
    menu = BoidsParamsMenu(boids.GetParams());
}

void BoidsDisplay::Initialise()
{
    boids.InitialiseBoids();
}

void BoidsDisplay::Update(Camera3D cam)
{
    boids.UpdateBoidPositions();

    if (menu.IsOpen())
    {
        menu.Update();
        return;
    }

    base.Update(Crosshair::GetPickRay(cam));
    if (base.IsHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        menu.Open();
    }
}

using Dimensions = Vector3;

static inline Dimensions GetDimsFromBoundingBox(BoundingBox box)
{
    return Vector3Subtract(box.max, box.min);
}

const Color BoidsDisplay::GLASS_COLOR = {0xc7, 0xfe, 0xff, 128};
const Color BoidsDisplay::BASE_COLOR = BLUE;

void BoidsDisplay::Draw()
{
    // draw all opaque content first; the glass case is translucent and must be drawn last,
    // with depth writes disabled, or its near wall would write its own (nearer) depth and
    // hide anything drawn behind it - like the base - even though it should show through
    rlBegin(RL_TRIANGLES);
        boids.DrawBoidsViaTriangles();
    rlEnd();

    base.Draw();
    baseLabel.Draw();

    Dimensions dims = GetDimsFromBoundingBox(boids.bounds);
    Vector3 center = Vector3Add(boids.bounds.min, Vector3Scale(dims, 0.5f));

    rlDisableDepthMask();
    DrawCube(center, dims.x, dims.y, dims.z, GLASS_COLOR);
    rlEnableDepthMask();
}

void BoidsDisplay::DrawTooltip()
{
    if (!menu.IsOpen())
    {
        base.DrawTooltip();
    }
}

void BoidsDisplay::DrawMenu() const
{
    menu.Draw();
}

bool BoidsDisplay::IsMenuOpen() const
{
    return menu.IsOpen();
}

}
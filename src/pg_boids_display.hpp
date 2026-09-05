#include "pg_boids.hpp"
#include "pg_hoverable_box.hpp"
namespace PG
{

class BoidsDisplay
{
public:
    BoidsDisplay() = default;
    BoidsDisplay(u32 numBoids, Vector3 position, float size, const BoidsParams& params);

    void Initialise();
    void Update(Camera3D cam);

    // draws the case and its contents; must be called within BeginMode3D/EndMode3D
    void Draw();

    // draws a tooltip if the base is hovered; must be called in 2D (outside of BeginMode3D/EndMode3D)
    void DrawTooltip();
private:
    Boids boids;
    Vector3 pos;
    float size;
    HoverableBox base;
    static const Color GLASS_COLOR;
    static const Color BASE_COLOR;
};

}
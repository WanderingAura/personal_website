#include "pg_boids.hpp"
#include "pg_hoverable_box.hpp"
#include "pg_boids_params_menu.hpp"
#include "pg_box_label.hpp"
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

    // draws the parameters menu if it's open; must be called in 2D (outside of BeginMode3D/EndMode3D)
    void DrawMenu() const;

    // true while the parameters menu is open; while true, callers should not drive camera
    // look/movement (the menu takes over the now-visible cursor) or show the crosshair
    bool IsMenuOpen() const;
private:
    Boids boids;
    Vector3 pos;
    float size;
    HoverableBox base;
    BoxLabel baseLabel;
    BoidsParamsMenu menu;
    static const Color GLASS_COLOR;
    static const Color BASE_COLOR;
};

}
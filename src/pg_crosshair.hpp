#include "raylib.h"

namespace PG
{

class Crosshair
{
public:
    // draws a crosshair at the centre of the screen, indicating what the camera is looking at.
    // must be called in 2D (i.e. outside of BeginMode3D/EndMode3D)
    static void Draw();

    // the ray from the camera through the centre of the screen, i.e. what the crosshair is
    // aiming at. Use this instead of a ray built from GetMousePosition() for hover/picking
    // logic: with DisableCursor() active for the free camera, GetMousePosition() is an
    // unbounded value that drifts as the mouse looks around and no longer corresponds to a
    // point on screen, so it can't be used to build a meaningful pick ray.
    static Ray GetPickRay(Camera3D cam);
};

}

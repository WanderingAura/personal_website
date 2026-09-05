#include "raylib.h"

namespace PG
{

class Crosshair
{
public:
    // draws a crosshair at the centre of the screen, indicating what the camera is looking at.
    // must be called in 2D (i.e. outside of BeginMode3D/EndMode3D)
    static void Draw();

    // ray from the camera through screen centre - what the crosshair aims at. Use this (not a
    // ray from GetMousePosition()) for hover/picking: with DisableCursor() active, the mouse
    // position drifts unbounded and no longer maps to a point on screen.
    static Ray GetPickRay(Camera3D cam);
};

}

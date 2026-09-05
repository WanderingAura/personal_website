#pragma once
#include "raylib.h"

namespace PG
{

// Renders text once to a texture and displays it as a decal on each of a box's 4 vertical
// side faces - e.g. a museum-placard-style label wrapped around a display case.
class BoxLabel
{
public:
    BoxLabel() = default;
    BoxLabel(BoundingBox bounds, const char* text);

    // must be called within BeginMode3D/EndMode3D
    void Draw() const;

private:
    BoundingBox bounds{};
    Texture2D texture{};
};

}

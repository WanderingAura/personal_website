#pragma once
#include "raylib.h"

namespace PG
{

// A framed picture: an image bordered by a wooden frame, mounted flat facing `normal` (e.g.
// hung on a wall, facing into the room), with a hover tooltip.
class Painting
{
public:
    Painting() = default;

    // position is the centre of the picture; normal is the direction it faces (e.g. away
    // from the wall it's mounted on - assumed roughly horizontal, like a wall-mounted
    // painting); size is the picture's width - its height is derived from imagePath's own
    // aspect ratio, so the image is scaled without being stretched. If linkUrl is given,
    // clicking the painting opens it in a new browser tab - web builds only, see Update()
    Painting(Vector3 position, Vector3 normal, float size, const char* imagePath, const char* tooltipText,
        const char* linkUrl = nullptr);

    // must be called every frame with the current pick ray (e.g. Crosshair::GetPickRay) to
    // keep the hover state up to date; also opens linkUrl (if set) on a left click while
    // hovered - only has an effect in web builds, since there's no browser tab to open otherwise
    void Update(Ray ray);

    // draws the image and its wooden frame; must be called within BeginMode3D/EndMode3D
    void Draw() const;

    // draws the tooltip if currently hovered; must be called in 2D (outside of BeginMode3D/EndMode3D)
    void DrawTooltip() const;

    bool IsHovered() const;

private:
    Vector3 position{};
    Vector3 normal{};
    // right x up == normal; computed once from normal so Draw()/Update() don't redo it
    Vector3 right{};
    Vector3 up{};
    float width = 0.0f;
    float height = 0.0f;
    Texture2D imageTexture{};
    Texture2D frameTexture{};
    const char* tooltipText = nullptr;
    const char* linkUrl = nullptr;
    bool isHovered = false;
};

}

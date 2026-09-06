#pragma once
#include "raylib.h"
#include "pg_video_texture.hpp"

namespace PG
{

// A framed picture: an image bordered by a wooden frame, mounted flat facing `normal` (e.g.
// hung on a wall, facing into the room), with a hover tooltip.
class Painting
{
public:
    Painting() = default;

    // position is the centre of the picture; normal is the direction it faces (e.g. away from
    // the wall it's mounted on); size is the picture's width - height follows imagePath's own
    // aspect ratio. If linkUrl is given, clicking the painting opens it in a new tab (web only).
    // If videoPath is given (web only - see VideoTexture), imagePath's image is shown as a
    // poster until hovered, then plays that video on loop for as long as it stays hovered
    Painting(Vector3 position, Vector3 normal, float size, const char* imagePath, const char* tooltipText,
        const char* linkUrl = nullptr, const char* videoPath = nullptr);

    // must be called every frame with the current pick ray (e.g. Crosshair::GetPickRay) to
    // keep the hover state up to date; also opens linkUrl (if set) on a left click while
    // hovered - only has an effect in web builds, since there's no browser tab to open otherwise
    void Update(Ray ray);

    // draws the image and its wooden frame; must be called within BeginMode3D/EndMode3D, and
    // while a lighting shader is active (see LightingSystem::BeginMode) - the image/frame should
    // be lit like the rest of the room
    void Draw() const;

    // draws the hover outline, if currently hovered; must be called within BeginMode3D/EndMode3D,
    // but OUTSIDE any lighting shader scope - it's an unlit, always-fully-bright highlight, and
    // drawing it while the room's lighting shader is active was found to corrupt lighting on
    // unrelated geometry (see LightingSystem::BeginMode/EndMode)
    void DrawHighlight() const;

    // draws the tooltip if currently hovered; must be called in 2D (outside of BeginMode3D/EndMode3D)
    void DrawTooltip() const;

    bool IsHovered() const;

private:
    // corners of the outer (frame-inclusive) rectangle, used for both hover ray collision
    // and the hover outline
    void GetOuterCorners(Vector3& bl, Vector3& br, Vector3& tr, Vector3& tl) const;

    Vector3 position{};
    Vector3 normal{};
    // right x up == normal; computed once from normal so Draw()/Update() don't redo it
    Vector3 right{};
    Vector3 up{};
    float width = 0.0f;
    float height = 0.0f;
    Texture2D imageTexture{};
    Texture2D frameTexture{};
    VideoTexture video;
    const char* tooltipText = nullptr;
    const char* linkUrl = nullptr;
    bool isHovered = false;
    bool wasHovered = false;
};

}

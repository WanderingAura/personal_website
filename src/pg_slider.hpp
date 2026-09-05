#pragma once
#include "raylib.h"

namespace PG
{

// A horizontal drag slider bound directly to a float value (e.g. a BoidsParams field), so
// moving the handle immediately writes through to whatever owns that value.
class Slider
{
public:
    Slider() = default;
    Slider(Rectangle bounds, const char* label, float min, float max, float* value);

    // handles click-and-drag; call every frame while the slider should be interactive
    // (requires the cursor to be visible, i.e. not under DisableCursor())
    void Update();

    // draws the track, filled portion, handle, and a "label: value" text above it
    void Draw() const;

private:
    Rectangle bounds{};
    const char* label = nullptr;
    float min = 0.0f;
    float max = 1.0f;
    float* value = nullptr;
    bool isDragging = false;
};

}

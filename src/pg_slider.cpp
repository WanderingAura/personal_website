#include "pg_slider.hpp"
#include <raymath.h>

namespace PG
{

static const constexpr int HANDLE_WIDTH = 6;
static const constexpr int LABEL_FONT_SIZE = 16;
static const constexpr int LABEL_MARGIN_BOTTOM = 4;

Slider::Slider(Rectangle bounds, const char* label, float min, float max, float* value) :
    bounds(bounds),
    label(label),
    min(min),
    max(max),
    value(value)
{
}

void Slider::Update()
{
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, bounds))
    {
        isDragging = true;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        isDragging = false;
    }

    if (isDragging)
    {
        float t = Clamp((mouse.x - bounds.x) / bounds.width, 0.0f, 1.0f);
        *value = Lerp(min, max, t);
    }
}

void Slider::Draw() const
{
    const char* text = TextFormat("%s: %.3f", label, *value);
    DrawText(text, (int)bounds.x, (int)bounds.y - LABEL_FONT_SIZE - LABEL_MARGIN_BOTTOM, LABEL_FONT_SIZE, WHITE);

    DrawRectangleRec(bounds, Fade(WHITE, 0.15f));
    DrawRectangleLinesEx(bounds, 1.0f, WHITE);

    float t = Clamp((*value - min) / (max - min), 0.0f, 1.0f);
    float fillWidth = bounds.width * t;
    DrawRectangle((int)bounds.x, (int)bounds.y, (int)fillWidth, (int)bounds.height, SKYBLUE);

    float handleX = bounds.x + fillWidth;
    DrawRectangle((int)handleX - HANDLE_WIDTH / 2, (int)bounds.y - 3, HANDLE_WIDTH, (int)bounds.height + 6, WHITE);
}

}

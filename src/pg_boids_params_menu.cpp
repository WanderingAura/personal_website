#include "pg_boids_params_menu.hpp"

namespace PG
{

static const constexpr float MENU_WIDTH = 420.0f;
static const constexpr float PADDING = 20.0f;
static const constexpr float TITLE_HEIGHT = 30.0f;
static const constexpr float ROW_HEIGHT = 45.0f;
static const constexpr float TRACK_HEIGHT = 18.0f;
static const constexpr int TITLE_FONT_SIZE = 20;

struct SliderSpec
{
    const char* label;
    float min;
    float max;
    float* value;
};

BoidsParamsMenu::BoidsParamsMenu(BoidsParams& params)
{
    float menuHeight = PADDING * 2.0f + TITLE_HEIGHT + NUM_SLIDERS * ROW_HEIGHT;
    bounds = Rectangle{
        (GetScreenWidth() - MENU_WIDTH) / 2.0f,
        (GetScreenHeight() - menuHeight) / 2.0f,
        MENU_WIDTH,
        menuHeight,
    };

    float trackX = bounds.x + PADDING;
    float trackWidth = MENU_WIDTH - PADDING * 2.0f;
    float firstRowY = bounds.y + PADDING + TITLE_HEIGHT;

    // every movement parameter gets a slider except boundsAvoidStrength; boidSize/boidSizeFraction
    // are appearance, not movement, so they're left out too
    SliderSpec specs[NUM_SLIDERS] = {
        { "Vicinity Distance",  0.1f,  5.0f,   &params.vicinityDist },
        { "Too Close Distance", 0.05f, 3.0f,   &params.tooCloseDist },
        { "Cohesion",           0.0f,  0.1f,   &params.cohesionFactor },
        { "Alignment",          0.0f,  0.1f,   &params.alignmentFactor },
        { "Repulsion",          0.0f,  0.1f,   &params.repulsionFactor },
        { "Speed",              1.0f,  100.0f, &params.speed },
    };

    for (u32 i = 0; i < NUM_SLIDERS; i++)
    {
        Rectangle trackBounds = { trackX, firstRowY + i * ROW_HEIGHT, trackWidth, TRACK_HEIGHT };
        sliders[i] = Slider(trackBounds, specs[i].label, specs[i].min, specs[i].max, specs[i].value);
    }
}

void BoidsParamsMenu::Open()
{
    isOpen = true;
    EnableCursor();
    // DisableCursor()'s virtual mouse position drifts unbounded while active, so the cursor
    // can reappear somewhere unexpected; re-centre it so it always starts inside the menu
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}

void BoidsParamsMenu::Close()
{
    isOpen = false;
    DisableCursor();
}

bool BoidsParamsMenu::IsOpen() const
{
    return isOpen;
}

void BoidsParamsMenu::Update()
{
    if (!isOpen) return;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        Close();
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), bounds))
    {
        Close();
        return;
    }

    for (u32 i = 0; i < NUM_SLIDERS; i++)
    {
        sliders[i].Update();
    }
}

void BoidsParamsMenu::Draw() const
{
    if (!isOpen) return;

    DrawRectangleRec(bounds, Fade(BLACK, 0.85f));
    DrawRectangleLinesEx(bounds, 2.0f, WHITE);

    const char* title = "Boids Parameters";
    int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(title, (int)(bounds.x + (bounds.width - titleWidth) / 2.0f), (int)(bounds.y + PADDING / 2.0f), TITLE_FONT_SIZE, WHITE);

    for (u32 i = 0; i < NUM_SLIDERS; i++)
    {
        sliders[i].Draw();
    }
}

}

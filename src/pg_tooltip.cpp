#include "pg_tooltip.hpp"
#include <raylib.h>

namespace PG
{

static const constexpr int PADDING = 8;
static const constexpr int FONT_SIZE = 16;
static const constexpr int MARGIN_FROM_BOTTOM = 20;

void Tooltip::DrawAtBottom(const char* text)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int textWidth = MeasureText(text, FONT_SIZE);
    int boxWidth = textWidth + PADDING * 2;
    int boxHeight = FONT_SIZE + PADDING * 2;

    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = screenHeight - boxHeight - MARGIN_FROM_BOTTOM;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.7f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);
    DrawText(text, boxX + PADDING, boxY + PADDING, FONT_SIZE, WHITE);
}

}

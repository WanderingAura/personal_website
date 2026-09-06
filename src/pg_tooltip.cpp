#include "pg_tooltip.hpp"
#include <raylib.h>
#include <algorithm>

namespace PG
{

static const constexpr int PADDING = 8;
static const constexpr int FONT_SIZE = 16;
static const constexpr int LINE_SPACING = 4;
static const constexpr int MARGIN_FROM_BOTTOM = 20;

void Tooltip::DrawAtBottom(const char* text)
{
    int lineCount = 0;
    const char** lines = (const char**)TextSplit(text, '\n', &lineCount);

    int maxLineWidth = 0;
    for (int i = 0; i < lineCount; i++)
    {
        maxLineWidth = std::max(maxLineWidth, MeasureText(lines[i], FONT_SIZE));
    }

    int lineHeight = FONT_SIZE + LINE_SPACING;
    int textHeight = lineHeight * lineCount - LINE_SPACING;
    int boxWidth = maxLineWidth + PADDING * 2;
    int boxHeight = textHeight + PADDING * 2;

    int boxX = (GetScreenWidth() - boxWidth) / 2;
    int boxY = GetScreenHeight() - boxHeight - MARGIN_FROM_BOTTOM;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.7f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);

    for (int i = 0; i < lineCount; i++)
    {
        int lineWidth = MeasureText(lines[i], FONT_SIZE);
        int lineX = boxX + (boxWidth - lineWidth) / 2;
        int lineY = boxY + PADDING + i * lineHeight;
        DrawText(lines[i], lineX, lineY, FONT_SIZE, WHITE);
    }
}

}

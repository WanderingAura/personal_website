#include "pg_resume_prompt.hpp"
#include <raylib.h>

namespace PG
{

static const constexpr int FONT_SIZE = 20;
static const constexpr int LINE_SPACING = 4;
static const constexpr char* LINE_1 = "Lost focus";
static const constexpr char* LINE_2 = "Click to resume!";

void ResumePrompt::Draw()
{
    int lineHeight = FONT_SIZE + LINE_SPACING;
    int totalHeight = lineHeight * 2 - LINE_SPACING;
    int startY = GetScreenHeight() / 2 - totalHeight / 2;

    int width1 = MeasureText(LINE_1, FONT_SIZE);
    DrawText(LINE_1, (GetScreenWidth() - width1) / 2, startY, FONT_SIZE, WHITE);

    int width2 = MeasureText(LINE_2, FONT_SIZE);
    DrawText(LINE_2, (GetScreenWidth() - width2) / 2, startY + lineHeight, FONT_SIZE, WHITE);
}

}

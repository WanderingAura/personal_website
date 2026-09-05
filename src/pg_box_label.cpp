#include "pg_box_label.hpp"
#include <raymath.h>
#include <rlgl.h>

namespace PG
{

static const constexpr int FONT_SIZE = 48;
static const constexpr int PADDING = 16;
// fraction of a face's width/height the label is allowed to fill, leaving a small margin
static const constexpr float FILL_FRACTION = 0.85f;
// how far off the box's surface to draw the decal, so it doesn't z-fight with the box itself
static const constexpr float SURFACE_OFFSET = 0.005f;

static Texture2D CreateTextTexture(const char* text)
{
    int textWidth = MeasureText(text, FONT_SIZE);
    int width = textWidth + PADDING * 2;
    int height = FONT_SIZE + PADDING * 2;

    // build via a CPU Image + LoadTextureFromImage (the same path LoadTexture(file) uses
    // internally) rather than a RenderTexture2D, so this texture's V axis follows the same
    // top-row-is-v0 convention as every other texture in this codebase - a RenderTexture2D's
    // colour attachment is stored GPU-side with the opposite (bottom-row-is-v0) convention
    Image image = GenImageColor(width, height, BLANK);
    ImageDrawText(&image, text, PADDING, PADDING, FONT_SIZE, WHITE);

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    return texture;
}

BoxLabel::BoxLabel(BoundingBox bounds, const char* text) :
    bounds(bounds),
    texture(CreateTextTexture(text))
{
}

namespace
{
    struct Face
    {
        Vector3 center;
        Vector3 uAxis;  // must satisfy uAxis x (0,1,0) == normal, so winding stays outward
        float width;
        Vector3 normal;
    };
}

void BoxLabel::Draw() const
{
    Vector3 dims = Vector3Subtract(bounds.max, bounds.min);
    Vector3 center = Vector3Add(bounds.min, Vector3Scale(dims, 0.5f));
    float aspect = (float)texture.width / (float)texture.height;

    Face faces[4] = {
        { { center.x, center.y, bounds.max.z },  { 1.0f, 0.0f, 0.0f },  dims.x, { 0.0f, 0.0f, 1.0f } },  // front (+Z)
        { { center.x, center.y, bounds.min.z },  { -1.0f, 0.0f, 0.0f }, dims.x, { 0.0f, 0.0f, -1.0f } }, // back (-Z)
        { { bounds.max.x, center.y, center.z },  { 0.0f, 0.0f, -1.0f }, dims.z, { 1.0f, 0.0f, 0.0f } },  // right (+X)
        { { bounds.min.x, center.y, center.z },  { 0.0f, 0.0f, 1.0f },  dims.z, { -1.0f, 0.0f, 0.0f } }, // left (-X)
    };

    rlSetTexture(texture.id);
    // rlgl's current color is leftover state from whatever was drawn before this (e.g. the
    // base's DrawCube call) and would tint the texture; reset it so the white text is unmodified
    rlColor4ub(255, 255, 255, 255);
    rlBegin(RL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            // fit the label within the face, preserving its aspect ratio, whichever
            // dimension (width or height) is the limiting one
            float decalWidth = faces[i].width * FILL_FRACTION;
            float decalHeight = decalWidth / aspect;
            if (decalHeight > dims.y * FILL_FRACTION)
            {
                decalHeight = dims.y * FILL_FRACTION;
                decalWidth = decalHeight * aspect;
            }

            Vector3 faceCenter = Vector3Add(faces[i].center, Vector3Scale(faces[i].normal, SURFACE_OFFSET));
            Vector3 halfU = Vector3Scale(faces[i].uAxis, decalWidth * 0.5f);
            Vector3 halfV = Vector3{ 0.0f, decalHeight * 0.5f, 0.0f };

            Vector3 bl = Vector3Subtract(Vector3Subtract(faceCenter, halfU), halfV);
            Vector3 br = Vector3Subtract(Vector3Add(faceCenter, halfU), halfV);
            Vector3 tr = Vector3Add(Vector3Add(faceCenter, halfU), halfV);
            Vector3 tl = Vector3Add(Vector3Subtract(faceCenter, halfU), halfV);

            rlNormal3f(faces[i].normal.x, faces[i].normal.y, faces[i].normal.z);
            // world-top must sample the image's top row (v=0); LoadTextureFromImage keeps the
            // image's natural row order, so that's the opposite mapping to a tiling texture
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(bl.x, bl.y, bl.z);
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(br.x, br.y, br.z);
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(tr.x, tr.y, tr.z);
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(tl.x, tl.y, tl.z);
        }
    rlEnd();
    rlSetTexture(0);
}

}

#include "pg_painting.hpp"
#include "pg_tooltip.hpp"
#include <raymath.h>
#include <rlgl.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace PG
{

static const constexpr char* FRAME_TEXTURE_PATH = "assets/bark_willow_diff_1k.jpg";
static const constexpr float FRAME_THICKNESS_FRACTION = 0.08f;
// how far the frame sticks out from the wall, in world units
static const constexpr float FRAME_DEPTH = 0.1f;
// world units per texture repeat for the frame's wood grain
static const constexpr float FRAME_TEXTURE_TILE_SIZE = 0.5f;
// how far off the wall to draw the picture/frame's back, so they don't z-fight with it
static const constexpr float SURFACE_OFFSET = 0.01f;

static Texture2D LoadPaintingTexture(const char* path, bool tiled)
{
    Texture2D texture = LoadTexture(path);
    if (tiled)
    {
        SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);
    }
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    return texture;
}

Painting::Painting(Vector3 position, Vector3 normal, float size, const char* imagePath, const char* tooltipText,
    const char* linkUrl) :
    position(position),
    normal(Vector3Normalize(normal)),
    width(size),
    tooltipText(tooltipText),
    linkUrl(linkUrl)
{
    // arbitrary reference perpendicular-ish to normal, just needs to not be parallel to it
    Vector3 reference = (fabsf(this->normal.y) > 0.99f) ? Vector3{ 1.0f, 0.0f, 0.0f } : Vector3{ 0.0f, 1.0f, 0.0f };
    right = Vector3Normalize(Vector3CrossProduct(reference, this->normal));
    up = Vector3CrossProduct(this->normal, right);

    imageTexture = LoadPaintingTexture(imagePath, false);
    height = size * ((float)imageTexture.height / (float)imageTexture.width);

    frameTexture = LoadPaintingTexture(FRAME_TEXTURE_PATH, true);
}

void Painting::Update(Ray ray)
{
    float frameThickness = width * FRAME_THICKNESS_FRACTION;
    float outerWidth = width + frameThickness * 2.0f;
    float outerHeight = height + frameThickness * 2.0f;

    Vector3 halfRight = Vector3Scale(right, outerWidth * 0.5f);
    Vector3 halfUp = Vector3Scale(up, outerHeight * 0.5f);

    Vector3 bl = Vector3Subtract(Vector3Subtract(position, halfRight), halfUp);
    Vector3 br = Vector3Subtract(Vector3Add(position, halfRight), halfUp);
    Vector3 tr = Vector3Add(Vector3Add(position, halfRight), halfUp);
    Vector3 tl = Vector3Add(Vector3Subtract(position, halfRight), halfUp);

    RayCollision collision = GetRayCollisionQuad(ray, bl, br, tr, tl);
    isHovered = collision.hit;

    if (isHovered && linkUrl && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
#if defined(PLATFORM_WEB)
        EM_ASM({ window.open(UTF8ToString($0), '_blank'); }, linkUrl);
#endif
    }
}

// emits one quad into the active RL_QUADS batch, centred at `center` and spanning `width`
// along `right` and `height` along `up`. Winding follows right x up == normal, so it's only
// visible (and only reads left-to-right/right-side-up) from the +normal side.
static void DrawFlatQuad(Vector3 center, Vector3 right, Vector3 up, float width, float height, Vector3 normal, float uMax, float vMax)
{
    Vector3 halfRight = Vector3Scale(right, width * 0.5f);
    Vector3 halfUp = Vector3Scale(up, height * 0.5f);

    Vector3 bl = Vector3Subtract(Vector3Subtract(center, halfRight), halfUp);
    Vector3 br = Vector3Subtract(Vector3Add(center, halfRight), halfUp);
    Vector3 tr = Vector3Add(Vector3Add(center, halfRight), halfUp);
    Vector3 tl = Vector3Add(Vector3Subtract(center, halfRight), halfUp);

    rlNormal3f(normal.x, normal.y, normal.z);
    // v=0 samples the source image's top row (raylib/GL convention), so world-top must be v=0
    rlTexCoord2f(0.0f, vMax); rlVertex3f(bl.x, bl.y, bl.z);
    rlTexCoord2f(uMax, vMax); rlVertex3f(br.x, br.y, br.z);
    rlTexCoord2f(uMax, 0.0f); rlVertex3f(tr.x, tr.y, tr.z);
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(tl.x, tl.y, tl.z);
}

// Draws a box sticking out of the wall by `depth` along `normal`, with `backCenter` as the
// face flush against the wall - i.e. the box spans normal from 0 (at the wall) to depth (its
// raised front). Right x up must equal normal. The back face is omitted since it's flush
// against the wall and never visible.
static void DrawFrameBox(Vector3 backCenter, Vector3 right, Vector3 up, Vector3 normal,
    float width, float height, float depth, float tileSize)
{
    Vector3 boxCenter = Vector3Add(backCenter, Vector3Scale(normal, depth * 0.5f));

    Vector3 frontCenter = Vector3Add(boxCenter, Vector3Scale(normal, depth * 0.5f));
    DrawFlatQuad(frontCenter, right, up, width, height, normal, width / tileSize, height / tileSize);

    // for each side face, the two axes passed as (right,up) to DrawFlatQuad are chosen so
    // their cross product equals that face's own outward normal (see DrawFlatQuad's contract)
    Vector3 topCenter = Vector3Add(boxCenter, Vector3Scale(up, height * 0.5f));
    DrawFlatQuad(topCenter, normal, right, depth, width, up, depth / tileSize, width / tileSize);

    Vector3 bottomCenter = Vector3Subtract(boxCenter, Vector3Scale(up, height * 0.5f));
    DrawFlatQuad(bottomCenter, right, normal, width, depth, Vector3Negate(up), width / tileSize, depth / tileSize);

    Vector3 leftCenter = Vector3Subtract(boxCenter, Vector3Scale(right, width * 0.5f));
    DrawFlatQuad(leftCenter, normal, up, depth, height, Vector3Negate(right), depth / tileSize, height / tileSize);

    Vector3 rightCenter = Vector3Add(boxCenter, Vector3Scale(right, width * 0.5f));
    DrawFlatQuad(rightCenter, up, normal, height, depth, right, height / tileSize, depth / tileSize);
}

void Painting::Draw() const
{
    Vector3 surfacePos = Vector3Add(position, Vector3Scale(normal, SURFACE_OFFSET));
    float frameThickness = width * FRAME_THICKNESS_FRACTION;
    float outerWidth = width + frameThickness * 2.0f;
    float outerHeight = height + frameThickness * 2.0f;

    rlSetTexture(imageTexture.id);
    rlColor4ub(255, 255, 255, 255);
    rlBegin(RL_QUADS);
        DrawFlatQuad(surfacePos, right, up, width, height, normal, 1.0f, 1.0f);
    rlEnd();
    rlSetTexture(0);

    rlSetTexture(frameTexture.id);
    rlColor4ub(255, 255, 255, 255);
    rlBegin(RL_QUADS);
        // top/bottom strips span the full outer width (so they cover the corners); left/right
        // strips just fill the remaining height between them - a simple non-mitred frame
        Vector3 topCenter = Vector3Add(surfacePos, Vector3Scale(up, (height + frameThickness) * 0.5f));
        DrawFrameBox(topCenter, right, up, normal, outerWidth, frameThickness, FRAME_DEPTH, FRAME_TEXTURE_TILE_SIZE);

        Vector3 bottomCenter = Vector3Subtract(surfacePos, Vector3Scale(up, (height + frameThickness) * 0.5f));
        DrawFrameBox(bottomCenter, right, up, normal, outerWidth, frameThickness, FRAME_DEPTH, FRAME_TEXTURE_TILE_SIZE);

        Vector3 leftCenter = Vector3Subtract(surfacePos, Vector3Scale(right, (width + frameThickness) * 0.5f));
        DrawFrameBox(leftCenter, right, up, normal, frameThickness, height, FRAME_DEPTH, FRAME_TEXTURE_TILE_SIZE);

        Vector3 rightCenter = Vector3Add(surfacePos, Vector3Scale(right, (width + frameThickness) * 0.5f));
        DrawFrameBox(rightCenter, right, up, normal, frameThickness, height, FRAME_DEPTH, FRAME_TEXTURE_TILE_SIZE);
    rlEnd();
    rlSetTexture(0);
}

void Painting::DrawTooltip() const
{
    if (isHovered)
    {
        Tooltip::DrawAtBottom(tooltipText);
    }
}

bool Painting::IsHovered() const
{
    return isHovered;
}

}

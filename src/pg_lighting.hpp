#pragma once
#include "raylib.h"
#include "pg_types.hpp"

namespace PG
{

enum class LightType
{
    Directional = 0,
    Point = 1,
};

struct Light
{
    LightType type;
    bool enabled = true;
    Vector3 position{};
    Vector3 target{};
    Color color = WHITE;

    // shader uniform locations for this light's fields within the `lights[]` array
    int enabledLoc = -1;
    int typeLoc = -1;
    int positionLoc = -1;
    int targetLoc = -1;
    int colorLoc = -1;
};

// Wraps a lighting shader (assets/shaders/gl*/lighting.vs+fs) and the fixed-size array of
// lights it supports. AddLight() can be called repeatedly (up to MAX_LIGHTS, matching the
// shader's `Light lights[MAX_LIGHTS]` uniform array) to add more lights later. Call
// Update() once per frame after moving the camera or any lights, and wrap draw calls that
// should be lit (i.e. whose geometry has normals, like Room) in BeginMode()/EndMode().
class LightingSystem
{
public:
    static const constexpr u32 MAX_LIGHTS = 4;

    LightingSystem() = default;

    // loads the shader; must be called after InitWindow (GL context must exist)
    void Initialise();

    // returns the light's index for later use with GetLight(); no-ops past MAX_LIGHTS
    u32 AddLight(LightType type, Vector3 position, Vector3 target, Color color);
    Light& GetLight(u32 index);

    // pushes the camera position and all lights' current values to the shader
    void Update(Camera3D cam);

    void BeginMode();
    void EndMode();

private:
    Shader shader{};
    Light lights[MAX_LIGHTS];
    u32 lightCount = 0;
};

}

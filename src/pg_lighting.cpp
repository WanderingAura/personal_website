#include "pg_lighting.hpp"

namespace PG
{

#if defined(PLATFORM_WEB)
static const constexpr char* VS_PATH = "assets/shaders/glsl100/lighting.vs";
static const constexpr char* FS_PATH = "assets/shaders/glsl100/lighting.fs";
#else
static const constexpr char* VS_PATH = "assets/shaders/glsl330/lighting.vs";
static const constexpr char* FS_PATH = "assets/shaders/glsl330/lighting.fs";
#endif

void LightingSystem::Initialise()
{
    shader = LoadShader(VS_PATH, FS_PATH);

    // "matModel"/"matNormal"/"colDiffuse"/"mvp" are auto-assigned by name on load; viewPos isn't
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
}

u32 LightingSystem::AddLight(LightType type, Vector3 position, Vector3 target, Color color)
{
    if (lightCount >= MAX_LIGHTS) return MAX_LIGHTS;

    u32 index = lightCount++;
    Light& light = lights[index];
    light.type = type;
    light.enabled = true;
    light.position = position;
    light.target = target;
    light.color = color;

    // NOTE: shader uniform names must match assets/shaders/*/lighting.fs's `lights[]` struct
    light.enabledLoc  = GetShaderLocation(shader, TextFormat("lights[%i].enabled", index));
    light.typeLoc     = GetShaderLocation(shader, TextFormat("lights[%i].type", index));
    light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", index));
    light.targetLoc   = GetShaderLocation(shader, TextFormat("lights[%i].target", index));
    light.colorLoc    = GetShaderLocation(shader, TextFormat("lights[%i].color", index));

    TraceLog(LOG_INFO, "[DEBUG] light %i added: type=%s pos=(%.2f, %.2f, %.2f) color=(%d,%d,%d,%d)",
        index, (type == LightType::Point) ? "Point" : "Directional",
        position.x, position.y, position.z, color.r, color.g, color.b, color.a);

    return index;
}

Light& LightingSystem::GetLight(u32 index)
{
    return lights[index];
}

void LightingSystem::Update(Camera3D cam)
{
    float viewPos[3] = { cam.position.x, cam.position.y, cam.position.z };
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], viewPos, SHADER_UNIFORM_VEC3);

    for (u32 i = 0; i < lightCount; i++)
    {
        Light& light = lights[i];

        int enabled = light.enabled ? 1 : 0;
        int type = (int)light.type;
        SetShaderValue(shader, light.enabledLoc, &enabled, SHADER_UNIFORM_INT);
        SetShaderValue(shader, light.typeLoc, &type, SHADER_UNIFORM_INT);

        float position[3] = { light.position.x, light.position.y, light.position.z };
        SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

        float target[3] = { light.target.x, light.target.y, light.target.z };
        SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

        float color[4] = { light.color.r / 255.0f, light.color.g / 255.0f, light.color.b / 255.0f, light.color.a / 255.0f };
        SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
    }
}

void LightingSystem::BeginMode()
{
    BeginShaderMode(shader);
}

void LightingSystem::EndMode()
{
    EndShaderMode();
}

}

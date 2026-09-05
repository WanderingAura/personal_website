#include "raylib.h"
#include "pg_types.hpp"
namespace PG
{

class Cone
{
public:
    // must be called within rlBegin(RL_TRIANGLES)
    static void DrawWithTriangles(Vector3 pos, Vector3 dir, float radius, float height, u32 slices, Color color);
};

}
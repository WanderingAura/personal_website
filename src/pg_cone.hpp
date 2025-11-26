#include "raylib.h"
#include "pg_types.hpp"
namespace PG
{

class Cone
{
public:
    // calculate the cone vertices and draw immediately
    static void DrawWithTriangles(Vector3 pos, Vector3 dir, float radius, float height, u32 slices, Color color);
};

}
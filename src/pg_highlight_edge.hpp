#pragma once
#include "raylib.h"

namespace PG
{

// Draws a solid rectangular "thick line" between two points, for hover-highlight outlines on
// geometry that isn't axis-aligned (so DrawCubeWires doesn't apply) and needs correct lighting
// while a custom shader is active - raylib's own DrawCylinderEx never calls rlNormal3f, so it
// silently reuses whatever normal was last set by unrelated earlier draw calls, producing wrong
// lighting wherever it's used. This instead sets a proper per-face normal, the same way this
// codebase's own wood-beam drawing does (see DrawTexturedBeam in pg_room.cpp). Round joints are
// filled with DrawSphere at each endpoint (which does set its own normals correctly).
void DrawHighlightEdge(Vector3 start, Vector3 end, float thickness, Color color);

}

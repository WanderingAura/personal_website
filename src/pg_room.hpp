#include "raylib.h"

namespace PG
{

// A cuboid room (floor + 4 walls, open top) with a square-based pyramid roof, built from
// textured quads/triangles whose normals face inward (walls/floor) or outward+up (roof). The
// walls are only meant to be seen from inside - standard backface culling hides them from
// outside - but the roof is meant to be seen from outside, sitting on top of the walls.
class Room
{
public:
    Room() = default;

    // position is the centre of the floor; size is the room's width/height/depth;
    // roofHeight is the height of the roof's apex above the top of the walls
    Room(Vector3 position, Vector3 size, const char* wallTexturePath, const char* floorTexturePath,
         const char* roofTexturePath, const char* beamTexturePath, float roofHeight);

    // must be called within BeginMode3D/EndMode3D
    void Draw() const;

private:
    Vector3 position;
    Vector3 size;
    float roofHeight;
    Texture2D wallTexture;
    Texture2D floorTexture;
    Texture2D roofTexture;
    Texture2D beamTexture;
};

}

#include "raylib.h"

namespace PG
{

// A cuboid room (floor + 4 walls, open top) built from textured quads whose normals face
// inward. It's only meant to be seen from inside - standard backface culling hides it from
// outside, and no attempt is made to make it look right from there.
class Room
{
public:
    Room() = default;

    // position is the centre of the floor; size is the room's width/height/depth
    Room(Vector3 position, Vector3 size, const char* wallTexturePath, const char* floorTexturePath);

    // must be called within BeginMode3D/EndMode3D
    void Draw() const;

private:
    Vector3 position;
    Vector3 size;
    Texture2D wallTexture;
    Texture2D floorTexture;
};

}

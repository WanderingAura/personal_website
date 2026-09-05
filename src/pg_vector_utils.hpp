#pragma once
#include "raylib.h"
#include <cmath>

namespace PG
{

// a vector guaranteed not to be (nearly) parallel to axis, for building a perpendicular basis
// via cross products
inline Vector3 NonParallelReference(Vector3 axis)
{
    return (fabsf(axis.y) > 0.99f) ? Vector3{ 1.0f, 0.0f, 0.0f } : Vector3{ 0.0f, 1.0f, 0.0f };
}

}

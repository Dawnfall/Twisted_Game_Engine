#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include <vector>

namespace Twisted
{
    struct LineVertex
    {
        Vec3f pos;
        Vec4f color;
    };
}

namespace Twisted::DebugDraw
{
    TWISTED_API void DrawLine(Vec3f a, Vec3f b, Vec4f color);
    TWISTED_API void DrawBox(const Mat4x4f& transform, Vec4f color);
    TWISTED_API void DrawWireSphere(Vec3f center, float radius, Vec4f color);
    TWISTED_API void DrawWireCapsule(Vec3f center, float halfHeight, float radius, Vec4f color,
                                     Quat rotation = Quat(1, 0, 0, 0));

    // Internal — called by LineRenderer only
    const std::vector<LineVertex>& GetLines();
    void Clear();
}

#include "DebugDraw.h"
#include <cmath>
#include <algorithm>

namespace Twisted::DebugDraw
{
    static std::vector<LineVertex> s_lines;

    void DrawLine(Vec3f a, Vec3f b, Vec4f color)
    {
        s_lines.push_back({ a, color });
        s_lines.push_back({ b, color });
    }

    void DrawBox(const Mat4x4f& transform, Vec4f color)
    {
        static constexpr Vec3f k_corners[8] = {
            {-1,-1,-1}, { 1,-1,-1}, { 1, 1,-1}, {-1, 1,-1},
            {-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1},
        };

        Vec3f w[8];
        for (int i = 0; i < 8; i++)
            w[i] = Vec3f(transform * Vec4f(k_corners[i], 1.0f));

        DrawLine(w[0], w[1], color); DrawLine(w[1], w[2], color);
        DrawLine(w[2], w[3], color); DrawLine(w[3], w[0], color);
        DrawLine(w[4], w[5], color); DrawLine(w[5], w[6], color);
        DrawLine(w[6], w[7], color); DrawLine(w[7], w[4], color);
        DrawLine(w[0], w[4], color); DrawLine(w[1], w[5], color);
        DrawLine(w[2], w[6], color); DrawLine(w[3], w[7], color);
    }

    void DrawWireSphere(Vec3f center, float radius, Vec4f color)
    {
        constexpr int   N    = 32;
        constexpr float step = 2.0f * 3.14159265f / N;

        for (int i = 0; i < N; i++)
        {
            float a0 = i * step,       a1 = (i + 1) * step;
            float c0 = cosf(a0) * radius, s0 = sinf(a0) * radius;
            float c1 = cosf(a1) * radius, s1 = sinf(a1) * radius;
            DrawLine(center + Vec3f(c0, s0,  0), center + Vec3f(c1, s1,  0), color);
            DrawLine(center + Vec3f(c0,  0, s0), center + Vec3f(c1,  0, s1), color);
            DrawLine(center + Vec3f( 0, c0, s0), center + Vec3f( 0, c1, s1), color);
        }
    }

    void DrawWireCapsule(Vec3f center, float halfHeight, float radius, Vec4f color, Quat rotation)
    {
        auto R = [&](Vec3f v) { return rotation * v; };

        constexpr int   N    = 32;
        constexpr float step = 2.0f * 3.14159265f / N;

        const Vec3f top    = center + R(Vec3f(0,  halfHeight, 0));
        const Vec3f bottom = center + R(Vec3f(0, -halfHeight, 0));

        // Top and bottom rings
        for (int i = 0; i < N; i++)
        {
            float a0 = i * step, a1 = (i + 1) * step;
            float x0 = cosf(a0) * radius, z0 = sinf(a0) * radius;
            float x1 = cosf(a1) * radius, z1 = sinf(a1) * radius;
            DrawLine(top    + R(Vec3f(x0, 0, z0)), top    + R(Vec3f(x1, 0, z1)), color);
            DrawLine(bottom + R(Vec3f(x0, 0, z0)), bottom + R(Vec3f(x1, 0, z1)), color);
        }

        // 4 side lines connecting the rings
        DrawLine(top + R(Vec3f( radius, 0,      0)), bottom + R(Vec3f( radius, 0,      0)), color);
        DrawLine(top + R(Vec3f(-radius, 0,      0)), bottom + R(Vec3f(-radius, 0,      0)), color);
        DrawLine(top + R(Vec3f(0,       0,  radius)), bottom + R(Vec3f(0,       0,  radius)), color);
        DrawLine(top + R(Vec3f(0,       0, -radius)), bottom + R(Vec3f(0,       0, -radius)), color);

        // Cap meridian arcs (half-circles in XY and ZY planes)
        const int halfN = N / 2;
        for (int i = 0; i < halfN; i++)
        {
            float a0 = i * step, a1 = (i + 1) * step; // covers 0..π
            float c0 = cosf(a0), s0 = sinf(a0);
            float c1 = cosf(a1), s1 = sinf(a1);

            DrawLine(top + R(Vec3f(c0 * radius,  s0 * radius, 0)),
                     top + R(Vec3f(c1 * radius,  s1 * radius, 0)), color);
            DrawLine(top + R(Vec3f(0,  s0 * radius, c0 * radius)),
                     top + R(Vec3f(0,  s1 * radius, c1 * radius)), color);
            DrawLine(bottom + R(Vec3f(c0 * radius, -s0 * radius, 0)),
                     bottom + R(Vec3f(c1 * radius, -s1 * radius, 0)), color);
            DrawLine(bottom + R(Vec3f(0, -s0 * radius, c0 * radius)),
                     bottom + R(Vec3f(0, -s1 * radius, c1 * radius)), color);
        }
    }

    const std::vector<LineVertex>& GetLines() { return s_lines; }
    void Clear() { s_lines.clear(); }
}

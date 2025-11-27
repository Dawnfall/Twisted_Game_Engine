#pragma once
#include "Utils/GlmUtils.h"
#include <vector>

namespace Twisted
{
    constexpr unsigned int POSITION_LAYOUT_LOC = 0;
    constexpr unsigned int NORMAL_LAYOUT_LOC = 1;
    constexpr unsigned int TEXCOORD_LAYOUT_LOC = 2;
    constexpr unsigned int COLOR_LAYOUT_LOC = 3;

    enum class MeshPrimitiveType
    {
        POINTS,
        LINES,
        TRIANGLES,
    };

    struct VertexAttribute
    {
        enum class Type
        {
            POSITION,
            NORMAL,
            TEXCOORD,
            COLOR
            // extendable
        };

        VertexAttribute(Type type, size_t count, size_t offset, unsigned int loc) :
            Type(type),
            Count(count),
            Offset(offset),
            Location(loc)
        {}

        Type Type;
        size_t Count;   // number of floats (3 for position/normal, 2 for texcoord, etc.)
        size_t Offset;  // offset in the interleaved buffer in floats
        unsigned int Location; // explicit shader location
    };

    struct PackedMeshData
    {
        std::vector<float> VertexBuffer;      // interleaved vertex data
        std::vector<VertexAttribute> Layout;  // description of attributes
        size_t Stride = 0;                     // stride in floats
        std::vector<unsigned int> Indices;    // index buffer
        MeshPrimitiveType PrimitiveType = MeshPrimitiveType::TRIANGLES;
    };

	struct TWISTED_API MeshData
	{
        std::vector<Vec3f> Positions;
        std::vector<Vec3f> Normals;
        std::vector<Vec2f> TexCoords;
        std::vector<Vec4f> Colors;

        std::vector<unsigned int> Indices;

        MeshPrimitiveType PrimitiveType = MeshPrimitiveType::TRIANGLES;

        size_t GetVertexCount() const { return Positions.size(); }

        void AddPosition(const Vec3f& pos) { Positions.emplace_back(pos); }
        void AddNormal(const Vec3f& normal) { Normals.emplace_back(normal); }
        void AddTexCoord(const Vec2f& uv) { TexCoords.emplace_back(uv); }
        void AddColor(const Vec4f& color) { Colors.emplace_back(color); }
        void AddIndex(unsigned int index) { Indices.emplace_back(index); }

        void Clear()
        {
            Positions.clear();
            Normals.clear();
            TexCoords.clear();
            Colors.clear();

            Indices.clear();
        }

        void RecalculateNormals();
        PackedMeshData Pack() const;
	};




}
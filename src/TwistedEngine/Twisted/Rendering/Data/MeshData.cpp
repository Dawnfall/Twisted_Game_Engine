#include "MeshData.h"

namespace Twisted
{
    void MeshData::RecalculateNormals() //this assumes triangles
    {
        if (Positions.empty() || Indices.empty())
            return;

        Normals.clear();
        Normals.resize(Positions.size(), glm::vec3(0.0f));

        for (size_t i = 0; i < Indices.size(); i += 3)
        {
            unsigned int i0 = Indices[i];
            unsigned int i1 = Indices[i + 1];
            unsigned int i2 = Indices[i + 2];

            const Vec3f& v0 = Positions[i0];
            const Vec3f& v1 = Positions[i1];
            const Vec3f& v2 = Positions[i2];

            Vec3f edge1 = v1 - v0;
            Vec3f edge2 = v2 - v0;
            Vec3f faceNormal = glm::normalize(glm::cross(edge1, edge2));

            Normals[i0] += faceNormal;
            Normals[i1] += faceNormal;
            Normals[i2] += faceNormal;
        }

        for (auto& normal : Normals)
            normal = glm::normalize(normal);
    }

    PackedMeshData MeshData::Pack() const
    {
        PackedMeshData packed;
        size_t vertexCount = Positions.size();  // assuming Positions.size() == Normals.size() == UVs.size() etc.

        // Layout
        size_t offset = 0;
        if (!Positions.empty())
        {
            packed.Layout.push_back({ VertexAttribute::Type::POSITION, 3, offset,POSITION_LAYOUT_LOC });
            offset += 3;
        }
        if (!Normals.empty())
        {
            packed.Layout.push_back({ VertexAttribute::Type::NORMAL, 3, offset,NORMAL_LAYOUT_LOC });
            offset += 3;
        }
        if (!TexCoords.empty())
        {
            packed.Layout.push_back({ VertexAttribute::Type::TEXCOORD, 2, offset,TEXCOORD_LAYOUT_LOC });
            offset += 2;
        }
        if (!Colors.empty())
        {
            packed.Layout.push_back({ VertexAttribute::Type::COLOR, 4, offset,COLOR_LAYOUT_LOC });
            offset += 4;
        }

        packed.Stride = offset;

        // Pack data
        packed.VertexBuffer.resize(vertexCount * packed.Stride);
        for (size_t i = 0; i < vertexCount; ++i)
        {
            size_t base = i * packed.Stride;
            size_t currentOffset = 0;

            if (!Positions.empty())
            {
                packed.VertexBuffer[base + currentOffset + 0] = Positions[i].x;
                packed.VertexBuffer[base + currentOffset + 1] = Positions[i].y;
                packed.VertexBuffer[base + currentOffset + 2] = Positions[i].z;
                currentOffset += 3;
            }

            if (!Normals.empty())
            {
                packed.VertexBuffer[base + currentOffset + 0] = Normals[i].x;
                packed.VertexBuffer[base + currentOffset + 1] = Normals[i].y;
                packed.VertexBuffer[base + currentOffset + 2] = Normals[i].z;
                currentOffset += 3;
            }

            if (!TexCoords.empty())
            {
                packed.VertexBuffer[base + currentOffset + 0] = TexCoords[i].x;
                packed.VertexBuffer[base + currentOffset + 1] = TexCoords[i].y;
                currentOffset += 2;
            }

            if (!Colors.empty())
            {
                packed.VertexBuffer[base + currentOffset + 0] = Colors[i].r;
                packed.VertexBuffer[base + currentOffset + 1] = Colors[i].g;
                packed.VertexBuffer[base + currentOffset + 2] = Colors[i].b;
                packed.VertexBuffer[base + currentOffset + 3] = Colors[i].a;
                currentOffset += 2;
            }
        }

        packed.Indices = Indices;
        packed.PrimitiveType = PrimitiveType;

        return packed;
    }

}
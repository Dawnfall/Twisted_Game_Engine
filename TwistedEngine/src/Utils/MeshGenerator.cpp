#include "MeshGenerator.h"

//#include "Twisted/Data/Vertex.h"
#include "Utils/GlmUtils.h"

#include <unordered_map>
#include <vector>

namespace Twisted
{
	const float PI_temp = 3.14f; //use somewhere else

	MeshData CreateIcosahedron(size_t iterCount)
	{
		const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

		std::vector<Vec3f> positions
		{
			{ -1, t, 0 },{ 1, t, 0 },{ -1, -t, 0 },{ 1, -t, 0 },
			{ 0, -1, t },{ 0, 1, t },{ 0, -1, -t },{ 0, 1, -t },
			{ t, 0, -1 },{ t, 0, 1 },{ -t, 0, -1 },{ -t, 0, 1 }
		};

		std::vector<unsigned int> indices
		{
			0, 11, 5,
			0, 5, 1,
			0, 1, 7,
			0, 7, 10,
			0, 10, 11,
			1, 5, 9,
			5, 11, 4,
			11, 10, 2,
			10, 7, 6,
			7, 1, 8,
			3, 9, 4,
			3, 4, 2,
			3, 2, 6,
			3, 6, 8,
			3, 8, 9,
			4, 9, 5,
			2, 4, 11,
			6, 2, 10,
			8, 6, 7,
			9, 8, 1
		};

		auto getMiddlePoint = [](unsigned int a, unsigned int b,
			std::unordered_map<uint64_t, unsigned int>& cache,
			std::vector<Vec3f>& verts) -> unsigned int 
			{
				uint64_t smaller = std::min(a, b);
				uint64_t greater = std::max(a, b);
				uint64_t key = (smaller << 32) | greater;

				if (auto it = cache.find(key); it != cache.end())
					return it->second;

				Vec3f middle = glm::normalize((verts[a] + verts[b]) * 0.5f);
				unsigned int index = static_cast<unsigned int>(verts.size());
				verts.push_back(middle);
				cache[key] = index;
				return index;
			};

		auto getUVs = [](const Vec3f& pos) ->Vec2f{
			float u = 0.5f + (std::atan2(pos.z, pos.x) / (2.0f * PI_temp));
			float v = 0.5f - (std::asin(pos.y) / PI_temp);
			return { u,v };
			};

		for (int iter = 0; iter < iterCount; iter++)
		{
			std::unordered_map<uint64_t, unsigned int> cache;
			std::vector<unsigned int> newIndices;
			for (int i = 0; i < indices.size(); i += 3)
			{
				unsigned int midAB = getMiddlePoint(indices[i], indices[i + 1],cache,positions);
				unsigned int midBC = getMiddlePoint(indices[i+1], indices[i + 2], cache, positions);
				unsigned int midCA = getMiddlePoint(indices[i+2], indices[i], cache, positions);

				newIndices.insert(newIndices.end(), { indices[i], midAB, midCA });
				newIndices.insert(newIndices.end(), { midAB, indices[i + 1], midBC });
				newIndices.insert(newIndices.end(), { midBC, indices[i + 2], midCA });
				newIndices.insert(newIndices.end(), { midCA, midAB, midBC });
			}
			indices = std::move(newIndices);
		}

		MeshData mesh;
		/*mesh.Indices = std::move(indices);

		for (auto& pos : positions)
		{
			Vertex vert;
			vert.Position = glm::normalize(pos);
			vert.Normal = vert.Position;
			vert.TexCoord = getUVs(vert.Position);

			mesh.Vertices.emplace_back(vert);
		}*/

		return mesh;
	}
}



#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include <vector>
#include <string>

namespace Twisted
{
	struct Model
	{
		struct MaterialData
		{
			std::string Name;
			SRef<Texture> Diffuse;
		};
		struct ModelNode
		{
			std::string Name;
			SRef<Mesh> Mesh;
			SRef<MaterialData> Material;
		};

		std::string Path;
		std::vector<ModelNode> Nodes;
	};
}
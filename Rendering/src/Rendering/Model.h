#pragma once
#include "AppCore.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include <vector>
#include <string>

namespace Twisted::Render
{
	struct TWISTED_API MaterialData
	{
		std::string Name;
		SRef<Texture> Diffuse;
	};

	struct TWISTED_API Model
	{
		struct TWISTED_API ModelNode
		{
			std::string Name;
			SRef<Mesh> Mesh;
			SRef<MaterialData> Material;
		};

		std::string Path;
		std::vector<ModelNode> Nodes;
	};
}
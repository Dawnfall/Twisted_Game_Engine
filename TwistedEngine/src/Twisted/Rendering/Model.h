#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/ObjectID.h"
#include "Mesh.h"
#include "Material.h"

namespace Twisted
{
	class TWISTED_API Model :public ObjectID
	{
	public:
		Model(uint64_t id) :
			ObjectID(id)
		{}

		std::vector<Material*> Materials;
		std::vector<Mesh*> Meshes;
	};
}
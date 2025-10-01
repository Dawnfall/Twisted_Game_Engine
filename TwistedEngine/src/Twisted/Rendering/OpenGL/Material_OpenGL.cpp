#include "Twisted/Rendering/Material.h"

#include "Twisted/Rendering/Shader.h"
#include <glad/glad.h>

namespace Twisted
{
	Material::Material(ObjectID id):
	BaseObject(id)
	{}

	Material::Material(ObjectID id, const MaterialData& materialData) :
		BaseObject(id),
		m_data(materialData)
	{
	}
}
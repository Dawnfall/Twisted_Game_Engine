#include "CRenderer.h"
#include "Twisted/TwistedMacros.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"

namespace Twisted
{
	void CRenderer::Serialize(BinSerializer& buffer)const
	{
		ObjectAssetEntry serMesh = AssetsRegistry::GetInstance().GetObjectEntry(m_mesh);
		ObjectAssetEntry serMaterial = AssetsRegistry::GetInstance().GetObjectEntry(m_material);

		buffer.Write<ObjectAssetEntry>(serMesh,nullptr);
		buffer.Write<ObjectAssetEntry>(serMaterial, nullptr);
	}

	void CRenderer::Deserialize(BinSerializer& buffer)
	{
		ObjectAssetEntry serMesh = buffer.Read<ObjectAssetEntry>(nullptr);
		ObjectAssetEntry serMaterial = buffer.Read<ObjectAssetEntry>(nullptr);
		
		m_mesh = AssetsRegistry::GetInstance().GetObjectFromEntry<Mesh>(serMesh);
		m_material = AssetsRegistry::GetInstance().GetObjectFromEntry<Material>(serMaterial);

	}
}

REGISTER_COMPONENT(CRenderer,"CRenderer");
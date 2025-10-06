#include "CRenderer.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
namespace Twisted
{
	void CRenderer::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		//ObjectAssetEntry serMesh = assetsLayer->GetObjectEntry(m_mesh);
		//ObjectAssetEntry serMaterial = assetsLayer->GetObjectEntry(m_material);

		//buffer.Write<ObjectAssetEntry>(serMesh);
		//buffer.Write<ObjectAssetEntry>(serMaterial);
	}

	void CRenderer::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		//ObjectAssetEntry serMesh = buffer.Read<ObjectAssetEntry>();
		//ObjectAssetEntry serMaterial = buffer.Read<ObjectAssetEntry>();
		
		//m_mesh = assetsLayer->GetObjectFromEntry(serMesh);
		//m_material = assetsLayer->GetObjectFromEntry(serMaterial);

	}
}
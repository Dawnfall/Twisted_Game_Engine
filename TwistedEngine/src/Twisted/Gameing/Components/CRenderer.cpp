#include "CRenderer.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

#include "Utils/YamlUtils.h"

namespace Twisted
{
	void CRenderer::Serialize(BinSerializer& buffer)const
	{
		auto serMeshInfo = AssetsLayer::GetInstance()->GetObjectAssetInfo(m_mesh);
		AssetUuid meshUuid = (serMeshInfo) ? serMeshInfo->GetUuid() : AssetUuid::Invalid();
		buffer.Write<AssetUuid>(meshUuid, nullptr);

		auto serMaterialInfo = AssetsLayer::GetInstance()->GetObjectAssetInfo(m_material);
		AssetUuid matUuid = (serMaterialInfo) ? serMaterialInfo->GetUuid() : AssetUuid::Invalid();
		buffer.Write<AssetUuid>(matUuid, nullptr);
	}
	void CRenderer::Deserialize(BinSerializer& buffer)
	{
		AssetUuid serMesh = buffer.Read<AssetUuid>(nullptr);
		std::string meshName = "";
		m_mesh = static_cast<Mesh*>(AssetsLayer::GetInstance()->GetAssetObject(serMesh, meshName));

		AssetUuid serMaterial = buffer.Read<AssetUuid>(nullptr);
		std::string materialName = "";
		m_material = static_cast<Material*>(AssetsLayer::GetInstance()->GetAssetObject(serMaterial, materialName));
	}

	YAML::Node CRenderer::YamlSerialize() const
	{
		YAML::Node node;

		node["mesh"] = YamlUtils::encodeTObject(m_mesh);
		node["mat"] = YamlUtils::encodeTObject(m_material);

		return node;
	}

	void CRenderer::YamlDeserialize(const YAML::Node& node)
	{
		m_mesh = static_cast<Mesh*>(YamlUtils::decodeTObject(node["mesh"]));
		m_material = static_cast<Material*>(YamlUtils::decodeTObject(node["mat"]));
	}
}




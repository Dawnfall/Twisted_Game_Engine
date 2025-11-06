#include "CRenderer.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

#include "Utils/YamlUtils.h"

namespace Twisted
{
	void CRenderer::Serialize(BinSerializer& buffer)const
	{
		//AssetUuid serMesh = AssetsLayer::GetInstance().GetObjectAsset(m_mesh);
		//AssetUuid serMaterial = AssetsLayer::GetInstance().GetObjectAsset(m_material);

		//buffer.Write<AssetUuid>(serMesh,nullptr);
		//buffer.Write<AssetUuid>(serMaterial, nullptr);
	}
	void CRenderer::Deserialize(BinSerializer& buffer)
	{
		//AssetUuid serMesh = buffer.Read<AssetUuid>(nullptr);
		//std::string meshName = "";
		//AssetUuid serMaterial = buffer.Read<AssetUuid>(nullptr);
		//std::string materialName = "";
		//
		//m_mesh = AssetsLayer::GetInstance().GetAssetObject<Mesh>(serMesh,meshName);
		//m_material = AssetsLayer::GetInstance().GetAssetObject<Material>(serMaterial, materialName);
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


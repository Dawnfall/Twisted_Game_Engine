#pragma once
#include "ImportedAsset.h"
#include "Utils/GlmUtils.h"
#include "Utils/WPtrBase.h"

#include <string>
#include <vector>

namespace Twisted
{
	struct MaterialSlot
	{
		std::string name;
		WPtrBase    material; // WPtr<Material> stored as WPtrBase
	};

	struct ModelNode
	{
		std::string               name;
		glm::mat4                 localTransform = glm::mat4(1.0f);
		WPtrBase                  mesh;          // WPtr<Mesh> stored as WPtrBase; empty if no geometry
		std::vector<MaterialSlot> materialSlots;
		std::vector<ModelNode>    children;
	};

	class TWISTED_API ModelAsset : public FileImportedAsset
	{
	public:
		using FileImportedAsset::FileImportedAsset;

		ModelNode&       GetRootNode()       { return m_rootNode; }
		const ModelNode& GetRootNode() const { return m_rootNode; }

	private:
		ModelNode m_rootNode;
	};
}

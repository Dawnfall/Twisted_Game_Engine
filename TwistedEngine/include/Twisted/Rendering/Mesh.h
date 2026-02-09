#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Twisted/Rendering/Data/MeshData.h"

namespace Twisted
{
	struct TWISTED_API Mesh :public TObject
	{
		Mesh(const std::string& name);
		void OnDestroy()override;
		void SetData(const PackedMeshData& packedData, MeshDrawType drawType);
	};
}


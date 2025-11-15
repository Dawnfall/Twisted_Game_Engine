#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"

namespace Twisted
{
	enum class MeshDrawType
	{
		DYNAMIC,
		STREAM,
		STATIC
	};

	enum class MeshWindingOrder
	{
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};


	struct TWISTED_API Mesh :public TObject
	{
		Mesh(const std::string& name);
		void OnDestroy()override;

		unsigned int Vao = 0;
		unsigned int Vbo = 0;
		unsigned int Ebo = 0;
		size_t IndexCount = 0;

		MeshWindingOrder WindOrder = MeshWindingOrder::CLOCKWISE;
		MeshDrawType DrawType = MeshDrawType::STATIC;
	};
}


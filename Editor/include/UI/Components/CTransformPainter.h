#pragma once

#include "UI/ComponentPainter.h"
#include "Components/CTransform.h"
#include "World.h"

#include "imgui.h"
#include <unordered_map>

namespace Twisted::Editor
{
	class CTransformPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj) override;

	private:
		std::unordered_map<TransformComponent*, Vec3f> m_eulerCache;
	};
}



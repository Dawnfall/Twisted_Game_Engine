#pragma once

#include "AppCore.h"

#include "Twisted/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"

namespace Twisted
{
	struct TWISTED_API FrameBuffer :public TObject
	{
		FrameBuffer(const std::string& name, Vec2i size);
		void OnCreate() override;
		void OnDestroy() override;

		void SetSize(const Vec2i& size);

	};
}


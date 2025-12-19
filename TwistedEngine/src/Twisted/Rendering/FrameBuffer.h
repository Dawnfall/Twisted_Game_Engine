#pragma once

#include "AppCore.h"

#include "Twisted/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"

namespace Twisted
{
	class TWISTED_API FrameBuffer :public TObject
	{
	public:
		FrameBuffer(const std::string& name, Vec2i size);
		void OnCreate() override;
		void OnDestroy() override;

		bool IsValid()const
		{
			return Id != 0 && Tex && Tex->IsValid();
		}

		Vec2i Size;
		WPtr<Texture> Tex;
		unsigned int Id = 0;
		unsigned int Rbo = 0;
		bool IsDirty = true;
	};
}


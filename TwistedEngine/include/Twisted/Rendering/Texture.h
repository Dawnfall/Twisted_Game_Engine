#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include <string>
#include "Utils/GlmUtils.h"
#include "Data/TextureData.h"

namespace Twisted
{
	struct TWISTED_API Texture :public TObject //assumes mipmap, rgba 4 channel input , assumes valid texture
	{
		Texture(const std::string& name);
		void OnDestroy() override;

		void SetData(const TextureData& data);
	};

}


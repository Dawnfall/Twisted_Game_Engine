#pragma once

#include "Twisted/Game/AComponent.h"

namespace Twisted
{
	enum class LightType
	{

	};

	class CLight :public AComponent
	{
	public:
		LightType Type;
	};
}
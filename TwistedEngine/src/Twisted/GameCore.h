#pragma once

#include "pch.h"
#include "Game/EcsManager.h"
//#include "Twisted/Game/Components/CTransform.h"


namespace Twisted
{
	class CTransform;

	class GameCore
	{
	public:
		EcsManager Ecs;
		std::vector<CTransform*> GetRootTransforms();

	private:
	};
}
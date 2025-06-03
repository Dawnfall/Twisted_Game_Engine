#pragma once
#include "Twisted/Game/SystemBase.h"

namespace Twisted
{
	class TWISTED_API SCameraController : public SystemBase
	{
	public:
		float m_moveSpeed = 0.01;
		float rotateSpeed = 0.05;

		void Update() override;
	};
}
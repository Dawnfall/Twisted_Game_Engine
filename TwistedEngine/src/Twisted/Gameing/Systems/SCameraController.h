#pragma once
#include "Twisted/Gameing/SystemBase.h"
//#include "Windowing/WindowLayer.h"

namespace Twisted
{
	class TWISTED_API SCameraController : public SystemBase
	{
	public:
		float m_moveSpeed = 0.01;
		float rotateSpeed = 0.05;

		void Update() override;
		//WindowLayer* m_windowLayer;

		std::string GetTypeName()const override{ return "SCameraController"; }

	};
}


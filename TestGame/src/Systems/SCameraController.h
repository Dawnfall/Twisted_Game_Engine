#pragma once

#include "Dawn.h"

class SCameraController : public Twisted::SystemBase
{
	float m_moveSpeed = 0.01;
	float rotateSpeed = 0.01;

	// Inherited via SystemBase
	void Update(Twisted::Application& app) override;
};
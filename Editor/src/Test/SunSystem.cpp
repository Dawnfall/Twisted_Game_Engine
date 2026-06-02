#include "Test/SunSystem.h"
#include "Test/SunComponent.h"
#include "Components/CTransform.h"
#include "World.h"

namespace Twisted::Editor
{
	void SunSystem::Update(float deltaTime)
	{
		SunComponent* sun = m_world->FindFirstOfType<SunComponent>();
		if (!sun)
			return;

		TransformComponent& transform = m_world->GetComponent<TransformComponent>(sun->GetID());
		transform.Rotate(Vec3f(0.0f, sun->rotationSpeed * deltaTime, 0.0f));
	}
}

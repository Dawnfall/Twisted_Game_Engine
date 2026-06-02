#include "Test/PlayerSystem.h"
#include "Test/PlayerComponent.h"
#include "Components/CTransform.h"
#include "World.h"
#include "Input.h"

namespace Twisted::Editor
{
	void PlayerSystem::Update(float deltaTime)
	{
		PlayerComponent* player = m_world->FindFirstOfType<PlayerComponent>();
		if (!player)
			return;

		TransformComponent& transform = m_world->GetComponent<TransformComponent>(player->GetID());
		Input& input = Input::GetInstance();

		// WASD movement on the XZ plane
		Vec3f move = Constants::Vec3Zero;
		if (input.GetKey(Key::W)) move += transform.GetForward();
		if (input.GetKey(Key::S)) move -= transform.GetForward();
		if (input.GetKey(Key::D)) move += transform.GetRight();
		if (input.GetKey(Key::A)) move -= transform.GetRight();

		move.y = 0.0f;
		if (glm::length2(move) > 0.0f)
			transform.Translate(glm::normalize(move) * player->moveSpeed * deltaTime);

		// Mouse look (yaw only — XZ movement plane stays flat)
		if (input.GetMouseButton(MouseButton::Right))
		{
			Vec2f delta = input.GetMouseDelta();
			if (delta.x != 0.0f)
			{
				Quat yaw = glm::angleAxis(-delta.x * player->lookSensitivity, Constants::Up);
				transform.LocalRot = glm::normalize(yaw * transform.LocalRot);
			}
		}
	}
}

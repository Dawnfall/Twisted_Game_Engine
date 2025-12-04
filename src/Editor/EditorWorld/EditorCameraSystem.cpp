#include "EditorCameraSystem.h"

namespace Twisted::Editor
{
	void EditorCameraSystem::FreeFlyCamera(TransformComponent& transform, float moveSpeed, float rotSpeed, float deltaTime)
	{
		auto& input = Input::GetInstance();

		if (input.GetMouseButton(MouseButton::Right))
		{
			HandleLocalWASD(transform, moveSpeed, deltaTime);
			HandleLocalMouseRot(transform, rotSpeed, deltaTime);
		}
	}

	void EditorCameraSystem::HandleLocalMouseRot(TransformComponent& transform, float rotSpeed, float deltaTime)
	{
		auto& input = Input::GetInstance();

		float xDelta = input.GetMouseDelta().x;
		float yDelta = input.GetMouseDelta().y;

		std::string out = "dX: " + std::to_string(xDelta)+"    dY: " + std::to_string(yDelta);
		TWISTED_INFO(out);
		
		if (xDelta != 0 || yDelta != 0)
		{
			Quat yaw = glm::angleAxis(-xDelta * rotSpeed * deltaTime, Constants::Up);
			Quat pitch = glm::angleAxis(-yDelta * rotSpeed * deltaTime, transform.GetRight());

			// yaw first, then pitch
			Quat deltaRot = yaw * pitch;

			transform.Rotate(deltaRot);
		}
	}

	void EditorCameraSystem::HandleLocalWASD(TransformComponent& transform, float moveSpeed, float deltaTime)
	{
		auto& input = Input::GetInstance();

		float forward = ((input.GetKey(Key::W)) ? 1.0f : 0.0f) + ((input.GetKey(Key::S)) ? -1.0f : 0.0f);
		float up = ((input.GetKey(Key::Q)) ? 1.0f : 0.0f) + ((input.GetKey(Key::E)) ? -1.0f : 0.0f);
		float right = ((input.GetKey(Key::D)) ? -1.0f : 0.0f) + ((input.GetKey(Key::A)) ? 1.0f : 0.0f);

		if (forward != 0.0f)
			transform.Translate(transform.GetForward() * forward * moveSpeed * deltaTime);
		if (up != 0.0f)
			transform.Translate(transform.GetUp() * up * moveSpeed * deltaTime);
		if (right != 0.0f)
			transform.Translate(transform.GetRight() * right * moveSpeed * deltaTime);
	}
}
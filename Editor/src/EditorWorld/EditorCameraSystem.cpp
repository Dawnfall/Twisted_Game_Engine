#include "EditorWorld/EditorCameraSystem.h"
#include "Input.h"
#include "Components/CTransform.h"
#include "ButtonCodes.h"
#include "Utils/GlmUtils.h"
#include "KeyCodes.h"
#include "EditorApp/EditorService.h"
#include "Application/Application.h"


namespace Twisted::Editor
{
	void EditorCameraSystem::FreeFlyCamera(TransformComponent& transform, float moveSpeed, float shiftMul,
	                                       float rotSpeed, float panSpeed, float scrollSpeed, float deltaTime)
	{
		auto* editorService = Application::GetInstance().GetService<EditorService>();
		if (!editorService)
			return;

		auto& input = Input::GetInstance();
		const Vec4f& r = editorService->WorldViewRect;
		const Vec2f  mouse = input.GetMousePosition();
		if (r.z <= 0.0f || mouse.x < r.x || mouse.y < r.y || mouse.x > r.z || mouse.y > r.w)
			return;

		float wheel = input.GetMouseWheelDelta();
		if (wheel != 0.0f)
			transform.Translate(transform.GetForward() * wheel * scrollSpeed);

		if (input.GetMouseButton(MouseButton::Right))
		{
			float speedMul = (input.GetKey(Key::LeftShift) || input.GetKey(Key::RightShift)) ? shiftMul : 1.0f;
			HandleLocalWASD(transform, moveSpeed * speedMul, deltaTime);
			HandleLocalMouseRot(transform, rotSpeed);
		}

		if (input.GetMouseButton(MouseButton::Middle))
			HandleMiddleMousePan(transform, panSpeed);
	}

	void EditorCameraSystem::HandleLocalMouseRot(TransformComponent& transform, float rotSpeed)
	{
		auto& input = Input::GetInstance();

		float xDelta = input.GetMouseDelta().x;
		float yDelta = input.GetMouseDelta().y;

		if (xDelta != 0 || yDelta != 0)
		{
			Quat yaw   = glm::angleAxis(-xDelta * rotSpeed, ::Constants::Up);
			Quat pitch = glm::angleAxis(yDelta * rotSpeed, transform.GetRight());
			transform.Rotate(yaw * pitch);
		}
	}

	void EditorCameraSystem::HandleMiddleMousePan(TransformComponent& transform, float panSpeed)
	{
		auto& input = Input::GetInstance();

		float xDelta = input.GetMouseDelta().x;
		float yDelta = input.GetMouseDelta().y;

		if (xDelta != 0 || yDelta != 0)
			transform.Translate(-transform.GetRight() * xDelta * panSpeed - transform.GetUp() * yDelta * panSpeed);
	}

	void EditorCameraSystem::HandleLocalWASD(TransformComponent& transform, float moveSpeed, float deltaTime)
	{
		auto& input = Input::GetInstance();

		float forward = ((input.GetKey(Key::W)) ? 1.0f : 0.0f) + ((input.GetKey(Key::S)) ? -1.0f : 0.0f);
		float up      = ((input.GetKey(Key::Q)) ? 1.0f : 0.0f) + ((input.GetKey(Key::E)) ? -1.0f : 0.0f);
		float right   = ((input.GetKey(Key::A)) ? 1.0f : 0.0f) + ((input.GetKey(Key::D)) ? -1.0f : 0.0f);

		if (forward != 0.0f)
			transform.Translate(transform.GetForward() * forward * moveSpeed * deltaTime);
		if (up != 0.0f)
			transform.Translate(transform.GetUp() * up * moveSpeed * deltaTime);
		if (right != 0.0f)
			transform.Translate(transform.GetRight() * right * moveSpeed * deltaTime);
	}
}

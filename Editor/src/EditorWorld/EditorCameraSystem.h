#pragma once
#include "SystemBase.h"
#include "Components/CCamera.h"
#include "Components/CTransform.h"
#include "World.h"


namespace Twisted::Editor
{
	class EditorCameraSystem :public SystemBase
	{
	public:
		EditorCameraSystem(World* world) :SystemBase(world) {}

		void Update(float deltaTime)override
		{
			CameraComponent* camera = m_world->FindFirstOfType<CameraComponent>();
			if (!camera)
				return;
			TransformComponent& transform = m_world->GetComponent<TransformComponent>(camera->GetID());

			constexpr float moveSpeed   = 5.0f;   // units/sec
			constexpr float shiftMul    = 5.0f;   // held-Shift multiplier
			constexpr float rotSpeed    = 0.003f; // rad/pixel (no deltaTime — delta is already frame-relative)
			constexpr float panSpeed    = 0.005f; // units/pixel
			constexpr float scrollSpeed = 1.0f;   // units/scroll-tick

			FreeFlyCamera(transform, moveSpeed, shiftMul, rotSpeed, panSpeed, scrollSpeed, deltaTime);
		}

	private:
		void HandleLocalWASD(TransformComponent& transform, float moveSpeed, float deltaTime);
		void HandleLocalMouseRot(TransformComponent& transform, float rotSpeed);
		void HandleMiddleMousePan(TransformComponent& transform, float panSpeed);

		void FreeFlyCamera(TransformComponent& transform, float moveSpeed, float shiftMul,
		                   float rotSpeed, float panSpeed, float scrollSpeed, float deltaTime);
	};
}
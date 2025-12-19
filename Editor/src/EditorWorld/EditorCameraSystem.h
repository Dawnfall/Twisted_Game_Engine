#pragma once
#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/World.h"


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

			float moveSpeed{ 1.0f };
			float rotateSpeed{ 0.005f };

			FreeFlyCamera(transform, moveSpeed, rotateSpeed, deltaTime);
		}

	private:
		void HandleLocalWASD(TransformComponent& transform, float moveSpeed, float deltaTime);
		void HandleLocalMouseRot(TransformComponent& transform, float rotSpeed, float deltaTime);

		void FreeFlyCamera(TransformComponent& transform, float moveSpeed, float rotSpeed, float deltaTime);
	};
}
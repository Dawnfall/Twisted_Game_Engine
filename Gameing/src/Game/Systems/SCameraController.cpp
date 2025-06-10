#include "Utils/GlmUtils.h"
#include "SCameraController.h"
#include "Game/World.h"
#include "Game/Components/CTransform.h"
#include "Game/Components/CCamera.h"
//#include "Windowing/KeyCodes.h"
#include "Twisted/Application/Application.h"

//#include "Windowing/InputManager.h"

namespace Twisted
{
	void SCameraController::Update()
	{
		/*auto view = m_world->GetComponents<CTransform, CCamera>();
		InputManager& input = m_windowLayer->GetInput();
		
		float forward = ((input.GetKey(TWISTED_KEY_W)) ? -1.0f : 0.0f) + ((input.GetKey(TWISTED_KEY_S)) ? 1.0f : 0.0f);
		float right = ((input.GetKey(TWISTED_KEY_D)) ? 1.0f : 0.0f) + ((input.GetKey(TWISTED_KEY_A)) ? -1.0f : 0.0f);
		Vec3f translateVec = Vec3f(right, 0.0f, forward);

		Vec3f eulerRot = Vec3f(input.GetMouseDelta().y, input.GetMouseDelta().x, 0.0f);

		for (auto&& [entity, transform, camera] : view.each())
		{
			Vec3f newPos = transform.LocalToWorldVector(translateVec * m_moveSpeed);
			transform.Translate(newPos);
			if (input.GetMouseButton(TWISTED_BUTTON_RIGHT))
			{
				transform.Rotate(eulerRot * rotateSpeed);
				Vec3f rotateVec = Vec3f(input.GetMouseDelta().y, input.GetMouseDelta().x, 0.0f);
				transform.Rotate(rotateVec * rotateSpeed);
			}
			transform.Rotate(Vec3f(0.0f, 0.01f, 0.0f) * m_world->GetApplication().GetTime().GetDeltaFrameTime());
		}*/
	}
}
#include "SCameraController.h"
#include "Dawn.h"

void SCameraController::Update(Twisted::AppBase* app)
{
	auto view = app->GetWorld()->GetComponents<Twisted::CTransform, Twisted::CCamera>();
	Twisted::InputManager& input = app->GetInput();

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
			//Twisted::Vec3f rotateVec = Twisted::Vec3f(mouseDelta.y, mouseDelta.x, 0.0f);
			//transform.Rotate(rotateVec * rotateSpeed);
		}
		//transform.Rotate(Twisted::Vec3f(0.0f, 0.01f, 0.0f));// *app.TimeManager.GetDeltaFrameTime());
	}
}

#include "SampleRuntime.h"
#include "Dawn.h"
#include "Systems/SCameraController.h"
#include <vector>
#include <string>

using namespace Twisted;

void SampleRuntime::OnBeforeRun(Twisted::AppBase* app)
{
	app->GetWorld()->AddSystem<SCameraController>();
	LoadEntities(app);
}


void SampleRuntime::LoadEntities(Twisted::AppBase* app)
{
	//Test(app);

	//******************************************************
	
	auto window = app->GetWindow();
	Twisted::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Twisted::Vec3f cubePos(0.0f, 0.0f, -2.0f);

	//camera
	
	EntityID cameraEntity =app->GetWorld()->CreateEntity<CCamera>(NullEntity);
	CTransform* camTransform = app->GetWorld()->GetComponent<CTransform>(cameraEntity);
	CCamera* camCamera = app->GetWorld()->GetComponent<CCamera>(cameraEntity);

	camTransform->SetLocalPosition(cameraPos);
	camTransform->SetLocalRotation(Twisted::Directions::IdentityQuat);
	camTransform->SetLocalScale(Twisted::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	camCamera->FovDeg = 45.0f;
	camCamera->AspectRatio = (float)window->Width / (float)window->Height;
	camCamera->FarPlane = 100.0f;
	camCamera->NearPlane = 0.1f;
	camCamera->CameraType = Twisted::CameraProjectionType::PERSPECTIVE;

	//cube
	EntityID cubeEntity = app->GetWorld()->CreateEntity<CRenderer>(NullEntity);
	CTransform* cubeTransform = app->GetWorld()->GetComponent<CTransform>(cubeEntity);
	CRenderer* cubeRenderer = app->GetWorld()->GetComponent<CRenderer>(cubeEntity);

	cubeTransform->SetWorldPosition(cubePos);
	cubeTransform->SetLocalRotation(Twisted::Directions::IdentityQuat);
	cubeTransform->SetLocalScale(Twisted::Directions::One);

	cubeRenderer->Mesh = app->GetResources().GetMesh(meshName);
	cubeRenderer->Material = app->GetResources().GetMaterial(Twisted::MATERIAL_DEFAULT_NAME);
}

void SampleRuntime::Test(AppBase* app)
{
	//TODO:... put to test somewhere 

	auto world = app->GetWorld();
	world->Clear();

	EntityID e1 = world->CreateEntity(NullEntity);
	EntityID e2 = world->CreateEntity<CRenderer, CLight>(NullEntity);

	CTransform* e1t = world->GetComponent<CTransform>(e1);

	CRenderer* ren = world->AddComponents<CRenderer>(e1);
	world->DestroyEntity(e1);
	world->DestroyOfType<CRenderer, CLight>();

	world->RemoveComponents<CRenderer, CLight>(e2);

	CTransform* e1Transform = world->GetComponent<CTransform>(e1);
	auto allTransforms = world->GetComponents<CTransform>();
	auto allCamerasAndTransforms = world->GetComponents<CTransform, CRenderer>();


}
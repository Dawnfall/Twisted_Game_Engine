#include "SampleRuntime.h"
#include "Dawn.h"
#include "Systems/SCameraController.h"
#include <vector>
#include <string>


void SampleRuntime::OnInit(Twisted::AppBase* app)
{
	app->CloseWindowEvent.AddListener([app]() { app->Stop(); });
}

void SampleRuntime::OnBeforeRun(Twisted::AppBase* app)
{
	app->GetWorld()->AddSystem<SCameraController>();

	LoadMaterials(app);
	LoadEntities(app);
}

void SampleRuntime::OnRun(Twisted::AppBase* app) {}

void SampleRuntime::LoadMaterials(Twisted::AppBase* app)
{
	auto mat = app->GetResources().CreateNewMaterial(materialName);
	mat->SetShader(app->GetResources().GetShader("SimpleShader"));
	mat->SetTexture("ourTexture", app->GetResources().GetTexture(wallImageName));
}

void SampleRuntime::LoadEntities(Twisted::AppBase* app)
{
	auto world = app->GetWorld();
	auto window = app->GetWindow();
	Twisted::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Twisted::Vec3f cubePos(0.0f, 0.0f, -2.0f);

	//camera
	entt::entity cameraID = world->GetEcs().CreateEntity();

	auto& camTrans = world->GetEcs().AddComponent<Twisted::CTransform>(cameraID);
	camTrans.SetLocalPosition(cameraPos);
	camTrans.SetLocalRotation(Twisted::Directions::IdentityQuat);
	camTrans.SetLocalScale(Twisted::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	auto& camCamera = world->GetEcs().AddComponent<Twisted::CCamera>(cameraID);
	camCamera.FovDeg = 45.0f;
	camCamera.AspectRatio = (float)window->Width / (float)window->Height;
	camCamera.FarPlane = 100.0f;
	camCamera.NearPlane = 0.1f;
	camCamera.CameraType = Twisted::CameraProjectionType::PERSPECTIVE;

	//cube
	entt::entity cubeEntity = world->GetEcs().CreateEntity();

	auto& cubeTrans = world->GetEcs().AddComponent<Twisted::CTransform>(cubeEntity);
	cubeTrans.SetWorldPosition(cubePos, world->GetEcs());
	cubeTrans.SetLocalRotation(Twisted::Directions::IdentityQuat);
	cubeTrans.SetLocalScale(Twisted::Directions::One);

	auto& cubeRenderer = world->GetEcs().AddComponent<Twisted::CRenderer>(cubeEntity);
	cubeRenderer.Mesh = app->GetResources().GetMesh(meshName);
	cubeRenderer.Material = app->GetResources().GetMaterial(materialName);
}
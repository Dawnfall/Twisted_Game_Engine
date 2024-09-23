#include "SampleRuntime.h"
#include "Dawn.h"
#include "Systems/SCameraController.h"
#include <vector>
#include <string>


void SampleRuntime::OnInit(Twisted::Application& app)
{
	app.WindowManager.closeWindowEvent.AddListener([&app]() { app.Stop(); });
}

void SampleRuntime::OnBeforeRun(Twisted::Application& app)
{
	app.Game.Systems.AddSystem<SCameraController>();

	LoadMaterials(app);
	LoadEntities(app);
}

void SampleRuntime::OnRun(Twisted::Application& app) {}


void SampleRuntime::LoadMaterials(Twisted::Application& app)
{
	auto mat = app.ResourceManager.CreateNewMaterial(materialName);
	mat->SetShader(app.ResourceManager.GetShader(Twisted::Collections::defaultShaderName));
	mat->SetTexture("ourTexture", app.ResourceManager.GetTexture(wallImageName));
}

void SampleRuntime::LoadEntities(Twisted::Application& app)
{
	Twisted::GameCore& gameCore = app.Game;

	Twisted::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Twisted::Vec3f cubePos(0.0f, 0.0f, -2.0f);

	//camera
	entt::entity cameraID = gameCore.CreateEntity();

	auto& camTrans = gameCore.AddComponent<Twisted::CTransform>(cameraID);
	camTrans.SetLocalPosition(cameraPos);
	camTrans.SetLocalRotation(Twisted::Collections::Directions::IdentityQuat);
	camTrans.SetLocalScale(Twisted::Collections::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	auto& camCamera = gameCore.AddComponent<Twisted::CCamera>(cameraID);
	camCamera.FovDeg = 45.0f;
	camCamera.AspectRatio = (float)app.WindowManager.GetWindow()->Width / (float)app.WindowManager.GetWindow()->Height;
	camCamera.FarPlane = 100.0f;
	camCamera.NearPlane = 0.1f;
	camCamera.CameraType = Twisted::CameraProjectionType::PERSPECTIVE;

	//cube
	entt::entity cubeEntity = gameCore.CreateEntity();

	auto& cubeTrans = gameCore.AddComponent<Twisted::CTransform>(cubeEntity);
	cubeTrans.SetWorldPosition(cubePos, app.Game);
	cubeTrans.SetLocalRotation(Twisted::Collections::Directions::IdentityQuat);
	cubeTrans.SetLocalScale(Twisted::Collections::Directions::One);

	auto& cubeRenderer = gameCore.AddComponent<Twisted::CRenderer>(cubeEntity);
	cubeRenderer.Mesh = app.ResourceManager.GetMesh(meshName);
	cubeRenderer.Material = app.ResourceManager.GetMaterial(materialName);
}
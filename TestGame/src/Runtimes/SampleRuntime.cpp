#include "SampleRuntime.h"
#include "Dawn.h"
#include "Systems/SCameraController.h"
#include <vector>
#include <string>


void SampleRuntime::OnInit(Twisted::Application* app)
{
	app->Windows.closeWindowEvent.AddListener([app]() { app->Stop(); });
}

void SampleRuntime::OnBeforeRun(Twisted::Application* app)
{
	app->Game.Ecs.AddSystem<SCameraController>();

	LoadMaterials(app);
	LoadEntities(app);
}

void SampleRuntime::OnRun(Twisted::Application* app) {}


void SampleRuntime::LoadMaterials(Twisted::Application* app)
{
	auto mat = app->Resources.CreateNewMaterial(materialName);
	mat->SetShader(app->Resources.GetShader("SimpleShader"));
	mat->SetTexture("ourTexture", app->Resources.GetTexture(wallImageName));
}

void SampleRuntime::LoadEntities(Twisted::Application* app)
{
	Twisted::GameCore& game = app->Game;
	auto window = app->Windows.GetWindow();
	Twisted::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Twisted::Vec3f cubePos(0.0f, 0.0f, -2.0f);

	//camera
	entt::entity cameraID = game.Ecs.CreateEntity();

	auto& camTrans = game.Ecs.AddComponent<Twisted::CTransform>(cameraID);
	camTrans.SetLocalPosition(cameraPos);
	camTrans.SetLocalRotation(Twisted::Directions::IdentityQuat);
	camTrans.SetLocalScale(Twisted::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	auto& camCamera = game.Ecs.AddComponent<Twisted::CCamera>(cameraID);
	camCamera.FovDeg = 45.0f;
	camCamera.AspectRatio = (float)window->Width / (float)window->Height;
	camCamera.FarPlane = 100.0f;
	camCamera.NearPlane = 0.1f;
	camCamera.CameraType = Twisted::CameraProjectionType::PERSPECTIVE;

	//cube
	entt::entity cubeEntity = game.Ecs.CreateEntity();

	auto& cubeTrans = game.Ecs.AddComponent<Twisted::CTransform>(cubeEntity);
	cubeTrans.SetWorldPosition(cubePos, app->Game.Ecs);
	cubeTrans.SetLocalRotation(Twisted::Directions::IdentityQuat);
	cubeTrans.SetLocalScale(Twisted::Directions::One);

	auto& cubeRenderer = game.Ecs.AddComponent<Twisted::CRenderer>(cubeEntity);
	cubeRenderer.Mesh = app->Resources.GetMesh(meshName);
	cubeRenderer.Material = app->Resources.GetMaterial(materialName);
}
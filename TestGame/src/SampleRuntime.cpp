#include "SampleRuntime.h"
#include "ShaderCollections.h"
#include "MeshCollections.h"
#include "Collections/Geometry.h"
#include <vector>
#include <string>


void SampleRuntime::OnInit(Twisted::Application& app)
{
	app.Game.Systems.AddSystem<Twisted::RenderSystem>();

	app.WindowManager.CreateNewWindow(window1Title, monitorWidth, monitorHeight);
	app.WindowManager.closeWindowEvent.AddListener([&app]() { app.Stop(); });
}

void SampleRuntime::OnBeforeRun(Twisted::Application& app)
{
	LoadMeshes(app);
	LoadShaders(app);
	LoadTextures(app);
	LoadMaterials(app);

	LoadEntities(app);
}

void SampleRuntime::OnRun(Twisted::Application& app) {}

void SampleRuntime::LoadMeshes(Twisted::Application& app)
{
	Twisted::MeshData meshData;
	meshData.Name = meshName;
	meshData.indices = Collections::quadIndices;
	meshData.vertices = Collections::quadVertices;

	app.ResourceManager.AddResource<Twisted::MeshData>(meshData);
	app.ResourceManager.CompileMeshes();
}

void SampleRuntime::LoadShaders(Twisted::Application& app)
{
	Twisted::ShaderData defaultShader;
	defaultShader.Name = shaderName;
	defaultShader.VertShaderCode = Collections::simpleVertexCode;
	defaultShader.FragShaderCode = Collections::simpleFragmentCode;
	//defaultShader.VertShaderCode = Collections::vertexShaderCode;
	//defaultShader.FragShaderCode = Collections::fragmentShaderCode;

	app.ResourceManager.AddResource(defaultShader);
	app.ResourceManager.CompileShaders(); //TODO: maybe not here
}

void SampleRuntime::LoadTextures(Twisted::Application& app)
{
	app.ResourceManager.CreateNewTexture(wallImagePath, wallImageName);
}


void SampleRuntime::LoadMaterials(Twisted::Application& app)
{
	auto mat = app.ResourceManager.CreateNewMaterial(materialName);
	mat->SetShader(app.ResourceManager.GetShader(shaderName));
	mat->SetTexture("ourTexture", app.ResourceManager.GetTexture(wallImageName));
}
//
void SampleRuntime::LoadEntities(Twisted::Application& app)
{
	Twisted::GameCore& gameCore = app.Game;

	Twisted::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Twisted::Vec3f trianglePos(0.0f, 0.0f, -2.0f);

	//camera
	entt::entity cameraID = gameCore.CreateEntity();

	auto& camTrans = gameCore.AddComponent<Twisted::CTransform>(cameraID);
	camTrans.SetLocalPosition(cameraPos);
	camTrans.SetLocalRotation(Twisted::Collections::Directions::IdentityQuat);
	camTrans.SetLocalScale(Twisted::Collections::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	auto& camCamera = gameCore.AddComponent<Twisted::CCamera>(cameraID);
	camCamera.FovDeg = 45.0f;
	camCamera.AspectRatio = (float)app.WindowManager.GetWindow(1)->Width / (float)app.WindowManager.GetWindow(1)->Height;
	camCamera.FarPlane = 100.0f;
	camCamera.NearPlane = 0.1f;
	camCamera.CameraType = Twisted::CameraProjectionType::PERSPECTIVE;

	//triangle
	entt::entity triangleEnt = gameCore.CreateEntity();

	auto& triTrans = gameCore.AddComponent<Twisted::CTransform>(triangleEnt);
	triTrans.SetWorldPosition(trianglePos, app.Game);
	triTrans.SetLocalRotation(Twisted::Collections::Directions::IdentityQuat);
	triTrans.SetLocalScale(Twisted::Collections::Directions::One);

	auto& triRend = gameCore.AddComponent<Twisted::CRenderer>(triangleEnt);
	triRend.Mesh = app.ResourceManager.GetMesh(meshName);
	triRend.Material = app.ResourceManager.GetMaterial(materialName);
}
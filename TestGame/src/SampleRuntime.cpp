#include "SampleRuntime.h"
#include "ShaderCollections.h"
#include "MeshCollections.h"
#include "Collections/Geometry.h"
#include <vector>
#include <string>


void SampleRuntime::OnInit(Dawn::Application& app)
{
	app.Game.Systems.AddSystem<Dawn::RenderSystem>();

	app.WindowManager.CreateNewWindow(window1Title, monitorWidth, monitorHeight);
	app.WindowManager.closeWindowEvent.AddListener([&app]() { app.Stop(); });
}

void SampleRuntime::OnBeforeRun(Dawn::Application& app)
{
	LoadMeshes(app);
	LoadShaders(app);
	LoadTextures(app);
	LoadMaterials(app);

	LoadEntities(app);
}

void SampleRuntime::OnRun(Dawn::Application& app) {}

void SampleRuntime::LoadMeshes(Dawn::Application& app)
{
	Dawn::MeshData meshData;
	meshData.Name = meshName;
	meshData.indices = Collections::quadIndices;
	meshData.vertices = Collections::quadVertices;

	app.ResourceManager.AddResource<Dawn::MeshData>(meshData);
	app.ResourceManager.CompileMeshes();
}

void SampleRuntime::LoadShaders(Dawn::Application& app)
{
	Dawn::ShaderData defaultShader;
	defaultShader.Name = shaderName;
	defaultShader.VertShaderCode = Collections::simpleVertexCode;
	defaultShader.FragShaderCode = Collections::simpleFragmentCode;
	//defaultShader.VertShaderCode = Collections::vertexShaderCode;
	//defaultShader.FragShaderCode = Collections::fragmentShaderCode;

	app.ResourceManager.AddResource(defaultShader);
	app.ResourceManager.CompileShaders(); //TODO: maybe not here
}

void SampleRuntime::LoadTextures(Dawn::Application& app)
{
	app.ResourceManager.CreateNewTexture(wallImagePath, wallImageName);
}


void SampleRuntime::LoadMaterials(Dawn::Application& app)
{
	auto mat = app.ResourceManager.CreateNewMaterial(materialName);
	mat->SetShader(app.ResourceManager.GetShader(shaderName));
	mat->SetTexture("ourTexture", app.ResourceManager.GetTexture(wallImageName));
}
//
void SampleRuntime::LoadEntities(Dawn::Application& app)
{
	Dawn::GameCore& gameCore = app.Game;

	Dawn::Vec3f cameraPos(0.0, 0.0f, 0.0f);
	Dawn::Vec3f trianglePos(0.0f, 0.0f, -2.0f);

	//camera
	entt::entity cameraID = gameCore.CreateEntity();

	auto& camTrans = gameCore.AddComponent<Dawn::CTransform>(cameraID);
	camTrans.SetLocalPosition(cameraPos);
	camTrans.SetLocalRotation(Dawn::Collections::Directions::IdentityQuat);
	camTrans.SetLocalScale(Dawn::Collections::Directions::One);
	//camTrans.LookAt(trianglePos, app.Game);

	auto& camCamera = gameCore.AddComponent<Dawn::CCamera>(cameraID);
	camCamera.FovDeg = 45.0f;
	camCamera.AspectRatio = (float)app.WindowManager.GetWindow(1)->Width / (float)app.WindowManager.GetWindow(1)->Height;
	camCamera.FarPlane = 100.0f;
	camCamera.NearPlane = 0.1f;
	camCamera.CameraType = Dawn::CameraProjectionType::PERSPECTIVE;

	//triangle
	entt::entity triangleEnt = gameCore.CreateEntity();

	auto& triTrans = gameCore.AddComponent<Dawn::CTransform>(triangleEnt);
	triTrans.SetWorldPosition(trianglePos, app.Game);
	triTrans.SetLocalRotation(Dawn::Collections::Directions::IdentityQuat);
	triTrans.SetLocalScale(Dawn::Collections::Directions::One);

	auto& triRend = gameCore.AddComponent<Dawn::CRenderer>(triangleEnt);
	triRend.Mesh = app.ResourceManager.GetMesh(meshName);
	triRend.Material = app.ResourceManager.GetMaterial(materialName);
}
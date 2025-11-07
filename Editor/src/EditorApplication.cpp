#include "EditorApplication.h"

#include "EditorLayer.h"
#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorConstants.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/BuiltIn/MeshCollections.h"
#include "Twisted/Rendering/FrameBuffer.h"

#include "Twisted/AssetsLayer/Project.h"

namespace Twisted::Editor
{
	void EditorApplication::OnCreate()
	{
		RegisterLayers();
	}

	void EditorApplication::OnBeforeRun()
	{
		EditorLayer::GetInstance().GetConfig().LoadConfig();
		m_assetsLayer->GetProject().ProjectChangeEvent.AddListener([this]() {
			layoutFilePath = (m_assetsLayer->GetProject().GetRootPath() / "EditorLayout.ini").string();
			Im::SetLayoutIniFile(layoutFilePath, true);

			m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsLayer->GetProject().GetName());
			LoadResources();
			});

		StartWindow();
		m_window->Maximize();
		m_window->SetPosition(EditorLayer::GetInstance().GetConfig().GetWindowPos());
	}

	void EditorApplication::OnFrame()
	{
		EditorLayer& editorData = EditorLayer::GetInstance();

		Window::PollEvents();
		m_window->GetContext()->Clear(Constants::WINDOW_CLEAR_COLOR);

		Im::StartFrame();

		editorData.GetInput().Update();
		m_renderLayer->Update();

		if (editorData.GetGameWorld())
			editorData.GetGameWorld()->UpdateFrame();

		m_editorLayer->Render(m_window);

		Im::EndFrame();
		m_window->GetContext()->SwapBuffers();
	}

	void EditorApplication::OnTerminate()
	{
		EditorLayer::GetInstance().GetConfig().SetWindowPos(m_window->GetPosition());
		EditorLayer::GetInstance().GetConfig().SetWindowSize(m_window->GetSize());

		EditorLayer::GetInstance().GetConfig().SaveConfig();

		ObjectManager::DestroyAll();
	}

	void EditorApplication::LoadBuiltIn()
	{
		auto& layer = AssetsLayer::GetInstance();

		Mesh* triangleMesh = ObjectManager::Create<Mesh>(Collections::triangleMeshName, MeshParams{});
		triangleMesh->SetData(Collections::triangleMesh);
		Mesh* quadMesh = ObjectManager::Create<Mesh>(Collections::quadMeshName, MeshParams{});
		quadMesh->SetData(Collections::quadMesh);
		Mesh* cubeMesh = ObjectManager::Create<Mesh>(Collections::cubeMeshName, MeshParams{});
		cubeMesh->SetData(Collections::cubeMesh);

		layer.AddBuiltIn(Collections::triangleMeshUUID, triangleMesh);
		layer.AddBuiltIn(Collections::quadMeshUUID, quadMesh);
		layer.AddBuiltIn(Collections::cubeMeshUUID, cubeMesh);
	}

	void EditorApplication::StartWindow()
	{
		m_window = m_windowLayer->CreateNewWindow(Constants::LOADUP_WIN_TITLE, Constants::LOADUP_WIN_SIZE, Constants::LOADUP_WIN_POS);
		Im::Init(m_windowLayer->GetWindow());
		Im::SetLayoutIniFile("");

		m_renderLayer->CreateFrameBuffer("Main Framebuffer", Vec2i(100, 100));
		m_window->CloseWindowEvent.AddListener([&]() {	Stop(); });
		m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
	}

	void EditorApplication::LoadResources()
	{
		m_assetsLayer->ImportAssets();
		LoadBuiltIn();
	}

	void EditorApplication::RegisterLayers()
	{
		m_windowLayer = AddLayer<WindowLayer>();
		m_renderLayer = AddLayer<RenderLayer>();
		m_assetsLayer = AddLayer<AssetsLayer>();
		m_editorLayer = AddLayer<EditorLayer>();
	}

	void EditorApplication::Create()
	{
		//for (const auto& pair : m_assetsLayer->GetAssets())
		//{
		//	pair.second->LoadCreate(m_registerLayer);
		//}

		//TODO:... post Load...assign pointers from uuids
		//TODO.... create temp object that hold pointer to new object and other data and can call postload()
	}
}
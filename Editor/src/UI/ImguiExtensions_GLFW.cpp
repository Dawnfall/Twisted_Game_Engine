#include "UI/ImguiExtensions.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>
#include <ImGuizmo.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Window.h"
#include "VulkanContext.h"

namespace Im
{
	void Init(Twisted::Window* window)
	{
		static bool isInit = false;
		if (isInit) return;
		isInit = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetRawPointer());
		ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);

		auto& ctx = Twisted::VK::VulkanContext::Get();
		ImGui_ImplVulkan_InitInfo info{};
		info.ApiVersion     = VK_API_VERSION_1_3;
		info.Instance       = ctx.Device.Instance;
		info.PhysicalDevice = ctx.Device.Physical;
		info.Device         = ctx.Device.Handle;
		info.QueueFamily    = ctx.Device.Queues.Graphics;
		info.Queue          = ctx.Device.GraphicsQueue;
		info.DescriptorPool = ctx.Descriptors.GlobalPool;
		info.MinImageCount  = 2;
		info.ImageCount     = static_cast<uint32_t>(ctx.Swapchain.Images.size());
		info.PipelineInfoMain.RenderPass = ctx.Swapchain.RenderPass;
		ImGui_ImplVulkan_Init(&info);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().IniFilename = nullptr;
		ImGui::StyleColorsDark();
	}

	void StartFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void Terminate()
	{
		auto& ctx = Twisted::VK::VulkanContext::Get();
		if (ctx.Device.Handle != VK_NULL_HANDLE)
			vkDeviceWaitIdle(ctx.Device.Handle);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

#include "ImguiExtensions.h"

#include <Windows.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Window.h"
#include "WindowsService.h"
#include "Vulkan/VulkanContext.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static VkDescriptorPool s_ImGuiPool = VK_NULL_HANDLE;

namespace Im
{
    void Init(Twisted::Window* window)
    {
        static bool isInit = false;
        if (isInit) return;
        isInit = true;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        HWND hwnd = static_cast<HWND>(window->GetRawPointer());
        ImGui_ImplWin32_Init(hwnd);

        // Provide surface creation for multi-viewport (ImGui_ImplWin32_Init always sets PlatformHasViewports)
        ImGui::GetPlatformIO().Platform_CreateVkSurface = [](ImGuiViewport* vp, ImU64 vk_inst, const void* vk_alloc, ImU64* out_surface) -> int
        {
            VkWin32SurfaceCreateInfoKHR ci{};
            ci.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            ci.hwnd      = static_cast<HWND>(vp->PlatformHandle);
            ci.hinstance = GetModuleHandle(nullptr);
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            VkResult err = vkCreateWin32SurfaceKHR(reinterpret_cast<VkInstance>(vk_inst), &ci,
                                                    static_cast<const VkAllocationCallbacks*>(vk_alloc),
                                                    &surface);
            *out_surface = reinterpret_cast<ImU64>(surface);
            return static_cast<int>(err);
        };

        window->GetWindowsService()->GetPollMsgEvent().AddListener([](void* rawMsg) {
            MSG* msg = static_cast<MSG*>(rawMsg);
            ImGui_ImplWin32_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        });

        auto& ctx = Twisted::VK::VulkanContext::Get();

        VkDescriptorPoolSize poolSize{};
        poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 100;

        VkDescriptorPoolCreateInfo poolCI{};
        poolCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCI.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolCI.maxSets       = 100;
        poolCI.poolSizeCount = 1;
        poolCI.pPoolSizes    = &poolSize;
        vkCreateDescriptorPool(ctx.Device, &poolCI, nullptr, &s_ImGuiPool);

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.ApiVersion     = VK_API_VERSION_1_3;
        initInfo.Instance       = ctx.Instance;
        initInfo.PhysicalDevice = ctx.PhysicalDevice;
        initInfo.Device         = ctx.Device;
        initInfo.QueueFamily    = ctx.Queues.Graphics;
        initInfo.Queue          = ctx.GraphicsQueue;
        initInfo.DescriptorPool = s_ImGuiPool;
        initInfo.MinImageCount  = Twisted::VK::FramesInFlight;
        initInfo.ImageCount     = Twisted::VK::FramesInFlight;
        initInfo.PipelineInfoMain.RenderPass   = ctx.RenderPass;
        initInfo.PipelineInfoMain.MSAASamples  = VK_SAMPLE_COUNT_1_BIT;
        ImGui_ImplVulkan_Init(&initInfo);

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::GetIO().IniFilename = nullptr;
        ImGui::StyleColorsDark();
    }

    void StartFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void Terminate()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (s_ImGuiPool != VK_NULL_HANDLE)
        {
            auto& ctx = Twisted::VK::VulkanContext::Get();
            vkDestroyDescriptorPool(ctx.Device, s_ImGuiPool, nullptr);
            s_ImGuiPool = VK_NULL_HANDLE;
        }
    }
}

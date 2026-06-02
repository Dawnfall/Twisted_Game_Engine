#ifdef _WIN32
#include "WIN32/NativeUtils_Win.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <shobjidl.h>

namespace Twisted::Windows
{
    std::filesystem::path OpenFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        HWND hwnd = glfwGetWin32Window(static_cast<GLFWwindow*>(window.GetRawPointer()));
        std::filesystem::path result;
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileDialog* pFileDialog = nullptr;
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pFileDialog));
            if (SUCCEEDED(hr))
            {
                DWORD options;
                pFileDialog->GetOptions(&options);
                pFileDialog->SetOptions(options | FOS_PATHMUSTEXIST);

                std::vector<COMDLG_FILTERSPEC> specs;
                for (const auto& f : filter)
                    specs.push_back({ f.first.c_str(), f.second.c_str() });
                if (!specs.empty())
                {
                    pFileDialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
                    pFileDialog->SetFileTypeIndex(1);
                }

                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            result = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileDialog->Release();
            }
            CoUninitialize();
        }
        return result;
    }

    std::filesystem::path OpenFolderDialog(Window& window)
    {
        HWND hwnd = glfwGetWin32Window(static_cast<GLFWwindow*>(window.GetRawPointer()));
        std::filesystem::path result;
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileDialog* pFileDialog = nullptr;
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pFileDialog));
            if (SUCCEEDED(hr))
            {
                DWORD options;
                pFileDialog->GetOptions(&options);
                pFileDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);

                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            result = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileDialog->Release();
            }
            CoUninitialize();
        }
        return result;
    }

    std::filesystem::path SaveFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        HWND hwnd = glfwGetWin32Window(static_cast<GLFWwindow*>(window.GetRawPointer()));
        std::filesystem::path result;
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileSaveDialog* pFileDialog = nullptr;
            hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileDialog));
            if (SUCCEEDED(hr))
            {
                std::vector<COMDLG_FILTERSPEC> specs;
                for (const auto& f : filter)
                    specs.push_back({ f.first.c_str(), f.second.c_str() });
                if (!specs.empty())
                {
                    pFileDialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
                    pFileDialog->SetFileTypeIndex(1);
                }

                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            result = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileDialog->Release();
            }
            CoUninitialize();
        }
        return result;
    }

    bool ShowConfirmDialog(Window& window, const std::wstring& message, const std::wstring& title)
    {
        HWND hwnd = glfwGetWin32Window(static_cast<GLFWwindow*>(window.GetRawPointer()));
        return MessageBoxW(hwnd, message.c_str(), title.c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES;
    }
}
#endif

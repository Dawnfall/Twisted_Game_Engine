#include "editorpch.h"
#include "NativeUtils.h"

#include <shobjidl.h> 
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // Must be included after glfw3.h

namespace Twisted::Native
{
    // filter: vector of {display name, pattern}, e.g. { {"Text Files", "*.txt"}, {"All Files", "*.*"} }
    std::string OpenFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        std::string result;
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

                // Set file type filters
                std::vector<COMDLG_FILTERSPEC> specs;
                for (const auto& f : filter)
                    specs.push_back({ f.first.c_str(), f.second.c_str() });
                if (!specs.empty())
                {
                    pFileDialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
                    pFileDialog->SetFileTypeIndex(1);
                }

                HWND hwnd = glfwGetWin32Window(window.GetPointer());
                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            char path[MAX_PATH];
                            wcstombs(path, pszFilePath, MAX_PATH);
                            result = path;
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

    std::string OpenFolderDialog(Window& window)
    {
        std::string result;
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

                HWND hwnd = glfwGetWin32Window(window.GetPointer());
                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            char path[MAX_PATH];
                            wcstombs(path, pszFilePath, MAX_PATH);
                            result = path;
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

    // filter: vector of {display name, pattern}, e.g. { {L"Text Files", L"*.txt"}, {L"All Files", L"*.*"} }
    std::filesystem::path SaveFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        std::filesystem::path result;
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileSaveDialog* pFileDialog = nullptr;
            hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileDialog));
            if (SUCCEEDED(hr))
            {
                // Set file type filters
                std::vector<COMDLG_FILTERSPEC> specs;
                for (const auto& f : filter)
                    specs.push_back({ f.first.c_str(), f.second.c_str() });
                if (!specs.empty())
                {
                    pFileDialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
                    pFileDialog->SetFileTypeIndex(1);
                }

                HWND hwnd = glfwGetWin32Window(window.GetPointer());
                if (SUCCEEDED(pFileDialog->Show(hwnd)))
                {
                    IShellItem* pItem = nullptr;
                    if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                    {
                        PWSTR pszFilePath = nullptr;
                        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
                        {
                            char path[MAX_PATH];
                            wcstombs(path, pszFilePath, MAX_PATH);
                            result = path;
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

    bool ShowConfirmDialog(HWND owner, const std::wstring& message, const std::wstring& title)
    {
        int result = MessageBoxW(
            owner,
            message.c_str(),
            title.c_str(),
            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2
        );
        return result == IDYES;
    }
}

//std::string OpenFileBrowser(Window& window, const std::string& filter)
//{
//	char filename[MAX_PATH] = { 0 };
//
//	OPENFILENAMEA ofn = { 0 };
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = glfwGetWin32Window(window.GetPointer());
//	ofn.lpstrFile = filename;
//	ofn.nMaxFile = MAX_PATH;
//	ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
//	ofn.nFilterIndex = 1;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //change last to change working dir
//	ofn.lpstrDefExt = "txt";
//
//	if (GetOpenFileNameA(&ofn))
//		return ofn.lpstrFile;
//	return "";
//}
//
//std::string SaveFileBrowser(Window& window, const std::string& filter)
//{
//	char filename[MAX_PATH] = { 0 };
//
//	OPENFILENAMEA ofn = { 0 };
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = glfwGetWin32Window(window.GetPointer());
//	ofn.lpstrFile = filename;
//	ofn.nMaxFile = MAX_PATH;
//	ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
//	ofn.nFilterIndex = 1;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //change last to change working dir
//	ofn.lpstrDefExt = "txt";
//
//	if (GetSaveFileNameA(&ofn))
//		return ofn.lpstrFile;
//	return "";
//}

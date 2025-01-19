project "TwistedEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20" --c++latest ??TODO??
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")
    
    pchheader "twistedpch.h"  -- This is the precompiled header file
    pchsource "src/twistedpch.cpp" -- This is the source file that compiles the precompiled header
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "TWISTED_ENGINE"
    }
    
    includedirs
    {
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLM}",
		"%{IncludeDirs.EnTT}",
        "%{IncludeDirs.Assimp}",
        "%{IncludeDirs.pugixml}",
        "%{IncludeDirs.stbi_image}",
    }

    links
    {
        "%{LibDirs.Assimp_debug}", --TODO...debug and release
        "Core",
        "Utils",
        "Rendering",
        "Glad",
        "GLFW",
        "pugixml",
        --default
        "opengl32.lib",
        "kernel32.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib"
    }

    filter "system:windows"       
        systemversion "latest"

        defines
        {
            "TWISTED_WINDOWS",
            "GLFW_INCLUDE_NONE",
            "GLFW_EXCLUDE_VULKAN"
        }

    filter "configurations:Debug"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

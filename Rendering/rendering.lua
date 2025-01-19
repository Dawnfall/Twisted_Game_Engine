project "Rendering"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "renderingpch.h"  -- This is the precompiled header file
    pchsource "src/renderingpch.cpp" -- This is the source file that compiles the precompiled header

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    links
    {
        "Core",
        "Utils",
        "Glad",
        "GLFW"
    }

    includedirs
    {
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.spdlog}", 
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.stbi_image}"
    }

    defines
    {
        "TWISTED_ENGINE"
    }

    filter "system:windows"
        defines
        {
            "TWISTED_WINDOWS"
        }
        systemversion "latest"

    filter "configurations:Debug"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

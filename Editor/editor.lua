project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "editorpch.h"  -- This is the precompiled header file
    pchsource "src/editorpch.cpp" -- This is the source file that compiles the precompiled header
    
    postbuildcommands {
        "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine.dll %{cfg.targetdir}",
     }
     
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    links
    {
        "TwistedEngine",
        "Core",
        "Utils",
        
        "Rendering",
        "GLFW",
        "Glad",

        "imgui"
    }

    includedirs
    {
        "%{IncludeDirs.Editor}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.EnTT}",
        "%{IncludeDirs.imgui}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "GLFW_EXCLUDE_VULKAN"
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

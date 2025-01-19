project "Standalone"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine.dll %{cfg.targetdir}",
        "{COPY} %{DllDirs.Assimp_debug} %{cfg.targetdir}"
     }

    files
    {
        "**.h",
        "**.cpp"
    }

    links
    {
        "TwistedEngine",
        "Core",
        "Utils",

        "Rendering",
        "GLFW",
        "Glad",
        
        "%{LibDirs.Assimp_debug}"
    }

    includedirs
    {
        "%{IncludeDirs.Standalone}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.EnTT}"
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

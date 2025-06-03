project "Standalone"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")


    files
    {
        "**.h",
        "**.cpp"
    }

    links
    {
        "Core",

        "Rendering",
        "TwistedEngine",
        "GLFW"
    }

    includedirs
    {
        "%{IncludeDirs.Standalone}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.GLFW}",
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
        targetname "Standalone_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"
        postbuildcommands {
            "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine_d.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Rendering/Rendering_d.dll %{cfg.targetdir}",
        }

    filter "configurations:Release"
        targetname "Standalone"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands {
            "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Rendering/Rendering.dll %{cfg.targetdir}",
        }

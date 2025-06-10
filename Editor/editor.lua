project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    links
    {
        "Core",
        
        "Windowing",
        "Rendering",
        "Gameing",
        "TwistedEngine",
        "GLFW",

        "imgui"
    }

    includedirs
    {
        "%{IncludeDirs.Editor}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Windowing}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.Gameing}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.GLFW}",
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
        targetname "Editor_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"
        postbuildcommands {
            "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine_d.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Windowing/Windowing_d.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Rendering/Rendering_d.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Gameing/Gameing_d.dll %{cfg.targetdir}"
        }

    filter "configurations:Release"
        targetname "Editor"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands {
            "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Windowing/Windowing_d.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Rendering/Rendering.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/bin/".. outputDir .. "/Gameing/Gameing.dll %{cfg.targetdir}"
        }
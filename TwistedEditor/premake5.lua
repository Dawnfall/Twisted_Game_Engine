project "TwistedEditor"
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
        "TwistedEngine",
        "imgui",
        "glfw",
        "%{LibDirs.Assimp}",
    }

    includedirs
    {
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.spdlog}",  
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.EnTT}",
        "%{IncludeDirs.Stbi}",
        "%{IncludeDirs.Assimp}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "GLFW_EXCLUDE_VULKAN"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "TWISTED_DEBUG"
        runtime "debug"
        symbols "on"

    filter "configurations:Release"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "TWISTED_DIST"
        runtime "Release"
        optimize "on"

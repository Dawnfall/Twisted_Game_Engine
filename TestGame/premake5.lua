project "TestGame"
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
        "GLFW"
    }

    includedirs
    {
        "%{IncludeDirs.TestGame}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.spdlog}",  
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.EnTT}",
        "%{IncludeDirs.Stbi}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.meta}"
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
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

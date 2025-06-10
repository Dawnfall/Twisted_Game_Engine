project "Windowing"
    kind "SharedLib"
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

    includedirs
    {
        "%{IncludeDirs.Windowing}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.spdlog}", 
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.TwistedEngine}"     
    }
    links
    {
        "Core",
        "GLFW"
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
        targetname "Windowing_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        targetname "Windowing"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

project "Rendering"
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

    links
    {
        "Core",
        "Glad"
    }

    includedirs
    {
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.spdlog}", 
        "%{IncludeDirs.GLM}"
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
        targetname "Rendering_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        targetname "Rendering"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

project "Utils"
    kind "StaticLib"
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
        "Core"
    }

    includedirs
    {
        "%{IncludeDirs.Utils}",
        "%{IncludeDirs.Core}",
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

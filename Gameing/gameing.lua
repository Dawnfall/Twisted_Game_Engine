project "Gameing"
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
       "Rendering"
       -- "Windowing"
    }

    includedirs
    {
        "%{IncludeDirs.Gameing}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.spdlog}", 
        "%{IncludeDirs.GLM}",
		"%{IncludeDirs.EnTT}"
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
        targetname "Gameing_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        targetname "Gameing"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

project "TestGame"
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
        "DawnEngine",
        "%{wks.location}/3rd/GLFW/lib/glfw3.lib"
    }

    includedirs
    {
        "%{wks.location}/DawnEngine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.spdlog}"     
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DAWN_DEBUG"
        runtime "debug"
        symbols "on"

    filter "configurations:Release"
        defines "DAWN_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DAWN_DIST"
        runtime "Release"
        optimize "on"

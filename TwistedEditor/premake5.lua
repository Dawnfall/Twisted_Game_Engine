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
        "%{wks.location}/3rd/GLFW/lib/glfw3.lib"
    }

    includedirs
    {
        "%{wks.location}/TwistedEngine/src",
        "%{IncludeDir.spdlog}",  
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.EnTT}",
        "%{IncludeDir.Stbi}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
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

project "TwistedEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20" --c++latest ??TODO??
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "TWISTED_ENGINE"
    }
    
    includedirs
    {
        "%{IncludeDirs.TwistedEngine}",
        "%{IncludeDirs.Core}",
        "%{IncludeDirs.Gameing}",
        "%{IncludeDirs.Rendering}",
        "%{IncludeDirs.Windowing}",
		"%{IncludeDirs.EnTT}",
        "%{IncludeDirs.spdlog}",
        --"%{IncludeDirs.GLFW}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.rapidobj}",
        "%{IncludeDirs.pugixml}",
        "%{IncludeDirs.stbi_image}",
    }

    links
    {
        "Core",
        "Windowing",
        "Rendering",
        "Gameing",
        --"GLFW",
        "pugixml",
        --default
        "opengl32.lib",
        "kernel32.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib"
    }

    filter "system:windows"       
        systemversion "latest"

        defines
        {
            "TWISTED_WINDOWS",
            "GLFW_INCLUDE_NONE",
            "GLFW_EXCLUDE_VULKAN"
        }

    filter "configurations:Debug"
        targetname "TwistedEngine_d"
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"
        optimize "off"

    filter "configurations:Release"
        targetname "TwistedEngine"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

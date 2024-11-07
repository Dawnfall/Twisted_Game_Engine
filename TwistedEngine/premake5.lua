project "TwistedEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")
    
    pchheader "twistedpch.h"  -- This is the precompiled header file
    pchsource "src/twistedpch.cpp" -- This is the source file that compiles the precompiled header
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    
    includedirs
    {
        "src",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLM}",
		"%{IncludeDirs.EnTT}",
        "%{IncludeDirs.Stbi}",
        "%{IncludeDirs.Assimp}",
        "%{IncludeDirs.meta}"
    }

    links
    {
        --"Assimp",
        "%{wks.location}/3rd/Assimp/lib/Debug/assimp-vc143-mtd.lib",
        "Glad",
        "GLFW",
        "imgui",
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
        defines "TWISTED_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "TWISTED_RELEASE"
        runtime "Release"
        optimize "on"

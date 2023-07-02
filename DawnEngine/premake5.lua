project "DawnEngine"
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

    includedirs
    {
    --  "%{IncludeDir.SPDLOG}",
    --  "%{IncludeDir.GLFWtemp}",
    --  "%{IncludeDir.GLFW},"
    --  "%{IncludeDir.Glad}"
    }

    links
    {
        --"%{wks.location}/DawnEngine/3rd/GLFWtemp/lib/glfw3.lib",
        -- "GLFW" -- ************project build not used currently due to some error

        "opengl32.lib"
        -- "Glad"
        -- "kernel32.lib",
        -- "user32.lib",
        -- "gdi32.lib",
        -- "winspool.lib",
        -- "comdlg32.lib",
        -- "advapi32.lib",
        -- "shell32.lib",
        -- "ole32.lib",
        -- "oleaut32.lib",
        -- "uuid.lib",
        -- "odbc32.lib",
        -- "odbccp32.lib"
    }

    filter "system:windows"       
        systemversion "latest"

        defines
        {
            "DAWN_WINDOWS",
            --"GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "DAWN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DAWN_RELEASE"
        runtime "Release"
        optimize "on"

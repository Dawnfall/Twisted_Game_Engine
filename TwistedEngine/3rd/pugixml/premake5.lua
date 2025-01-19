project "pugixml"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    targetdir("bin" .. outputDir .. "/%{prj.name}")
    objdir("bin-int".. outputDir.."/%{prj.name}")

    files
    {
        "include/pugixml/**.cpp",
        "include/pugixml/**.hpp"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
workspace "DawnEngine"
    architecture "x64"
    startproject "TestGame"
    
    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["spdlog"] ="%{wks.location}/3rd/spdlog/include"
    IncludeDir["GLFW"] = "%{wks.location}/3rd/GLFW/include"
    IncludeDir["Glad"]="%{wks.location}/DawnEngine/3rd/Glad/include"

    group "Dependecies"
        include "DawnEngine/3rd/Glad" -- --    include "DawnEngine/3rd/GLFW"
    group ""

    include "DawnEngine"
    include "TestGame"
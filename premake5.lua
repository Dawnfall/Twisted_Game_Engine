workspace "DawnEngine"
    architecture "x64"
    startproject "DawnEditor"
    
    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["spdlog"] ="%{wks.location}/3rd/spdlog/include"
    IncludeDir["GLFW"] = "%{wks.location}/3rd/GLFW/include"
    IncludeDir["Glad"] = "%{wks.location}/DawnEngine/3rd/Glad/include"
    IncludeDir["GLM"] = "%{wks.location}/3rd/GLM/glm"
	IncludeDir["EnTT"] = "%{wks.location}/DawnEngine/3rd/EnTT"
    IncludeDir["Stbi"]= "%{wks.location}/3rd/stb_image"

    group "Dependecies"
        include "DawnEngine/3rd/Glad" -- --    include "DawnEngine/3rd/GLFW"
    group ""

    include "DawnEngine"
    include "DawnEditor"
    include "TestGame"
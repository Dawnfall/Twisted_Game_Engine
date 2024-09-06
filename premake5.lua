workspace "TwistedEngine"
    architecture "x64"
    startproject "TwistedEditor"
    
    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["spdlog"] ="%{wks.location}/3rd/spdlog/include"
    IncludeDir["GLFW"] = "%{wks.location}/3rd/GLFW/include"
    IncludeDir["Glad"] = "%{wks.location}/TwistedEngine/3rd/Glad/include"
    IncludeDir["GLM"] = "%{wks.location}/3rd/GLM/glm"
	IncludeDir["EnTT"] = "%{wks.location}/TwistedEngine/3rd/EnTT"
    IncludeDir["Stbi"]= "%{wks.location}/3rd/stb_image"
    IncludeDir["Assimp"]="%{wks.location}/3rd/Assimp/include"
    IncludeDir["imgui"]="%{wks.location}/3rd/imgui/include"
    
    group "Dependecies"
        include "TwistedEngine/3rd/Glad" -- --    include "TwistedEngine/3rd/GLFW"
        include "3rd/imgui"
    group ""

    include "TwistedEngine"
    include "TwistedEditor"
    include "TestGame"
    
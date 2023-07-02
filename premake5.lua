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
--IncludeDir["SPDLOG"] ="%{wks.location}/DawnEngine/3rd/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/3rd/GLFW/include"
--IncludeDir["GLFWtemp"]="%{wks.location}/DawnEngine/3rd/GLFWtemp/include"
--IncludeDir["Glad"]="%{wks.location}/DawnEngine/3rd/Glad/include"

--group "Dependecies"
--    include "DawnEngine/3rd/GLFW"
--    include "DawnEngine/3rd/Glad"
--group ""

include "DawnEngine"
include "TestGame"
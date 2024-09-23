workspace "TwistedEngine"
    architecture "x64"
    startproject "TwistedEditor"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "includes.lua"
    include "libs.lua"
    
    group "Dependecies"
        include "3rd/GLFW"
        include "3rd/imgui"   
        include "TwistedEngine/3rd/Glad"
    group ""

    include "TwistedEngine"
    include "TestGame"
    
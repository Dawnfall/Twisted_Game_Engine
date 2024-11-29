workspace "TwistedEngine"
    architecture "x64"
    startproject "TestGame"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "buildData.lua"
    
    group "Dependecies"
        include "3rd/GLFW"
        include "3rd/imgui"   
        include "TwistedEngine/3rd/Glad"
    group ""

    include "TwistedEngine/engine.lua"
    include "Editor/editor.lua"
    include "TestGame/test.lua"
    include "Standalone/standalone.lua"
    --TODO: rename file
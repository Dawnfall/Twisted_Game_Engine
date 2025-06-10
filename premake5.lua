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
        include "3rd/Glad"
        include "3rd/pugixml"
    group ""

    group "Engine"
        include "Core/core.lua"
        include "Windowing/windowing.lua"
        include "Rendering/rendering.lua"
        include "Gameing/gameing.lua"
        include "TwistedEngine/engine.lua"
    group ""

    group "Editor"
        include "Editor/editor.lua"
    group ""

    include "Standalone/standalone.lua"
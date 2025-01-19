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
        include "TwistedEngine/3rd/pugixml"
    group ""

    group "Engine"
        include "Core/core.lua"
        include "Utils/utils.lua"
        include "Rendering/rendering.lua"
        include "TwistedEngine/engine.lua"
    group ""

    group "Editor"
        include "Editor/editor.lua"
    group ""

    include "TestGame/test.lua"
    include "Standalone/standalone.lua"
    --TODO: rename file
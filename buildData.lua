-- Includes

IncludeDirs = {}
IncludeDirs["Core"]= "%{wks.location}/Core/src";
IncludeDirs["Utils"]= "%{wks.location}/Utils/src";
IncludeDirs["Rendering"]= "%{wks.location}/Rendering/src";
IncludeDirs["TwistedEngine"]="%{wks.location}/TwistedEngine/src";
IncludeDirs["Editor"]="%{wks.location}/Editor/src";
IncludeDirs["TestGame"]="%{wks.location}/TestGame/src";
IncludeDirs["Standalone"]="%{wks.location}/Standalone/src";

-----3rd
IncludeDirs["imgui"]="%{wks.location}/3rd/imgui"
IncludeDirs["GLM"] = "%{wks.location}/3rd/GLM/glm" 
IncludeDirs["GLFW"] = "%{wks.location}/3rd/GLFW/include" --move to engine
IncludeDirs["spdlog"] ="%{wks.location}/3rd/spdlog/include" --move to engine
IncludeDirs["pugixml"]="%{wks.location}/TwistedEngine/3rd/pugixml/include"
IncludeDirs["Assimp"]="%{wks.location}/TwistedEngine/3rd/Assimp/include"
IncludeDirs["Glad"] = "%{wks.location}/TwistedEngine/3rd/Glad/include"
IncludeDirs["EnTT"] = "%{wks.location}/TwistedEngine/3rd/EnTT/include"
IncludeDirs["stbi_image"]= "%{wks.location}/TwistedEngine/3rd/stbi_image/include"

-- Dlls

DllDirs={}
DllDirs["Assimp_debug"]="%{wks.location}/TwistedEngine/3rd/Assimp/lib/Debug/assimp-vc143-mtd.dll";
DllDirs["Assimp_release"]="%{wks.location}/TwistedEngine/3rd/Assimp/lib/x64/assimp-vc143-mtd.dll";

-- Libs

LibDirs={}
LibDirs["GLFW"]="%{wks.location}/3rd/GLFW/lib/glfw3.lib"  
LibDirs["Assimp_debug"]="%{wks.location}/TwistedEngine/3rd/Assimp/lib/Debug/assimp-vc143-mtd.lib";
LibDirs["Assimp_release"]="%{wks.location}/TwistedEngine/3rd/Assimp/lib/x64/assimp-vc143-mtd.lib";



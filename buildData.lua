-- Includes

IncludeDirs = {}
IncludeDirs["Core"]= "%{wks.location}/Core/src";
IncludeDirs["Windowing"]= "%{wks.location}/Windowing/src";
IncludeDirs["Rendering"]= "%{wks.location}/Rendering/src";
IncludeDirs["Gameing"]="%{wks.location}/Gameing/src";
IncludeDirs["TwistedEngine"]="%{wks.location}/TwistedEngine/src";
IncludeDirs["Editor"]="%{wks.location}/Editor/src";
IncludeDirs["Standalone"]="%{wks.location}/Standalone/src";

-----3rd
IncludeDirs["imgui"]="%{wks.location}/3rd/imgui"
IncludeDirs["GLM"] = "%{wks.location}/3rd/GLM/glm" 
IncludeDirs["GLFW"] = "%{wks.location}/3rd/GLFW/include" --move to engine
IncludeDirs["spdlog"] ="%{wks.location}/3rd/spdlog/include" --move to engine
IncludeDirs["pugixml"]="%{wks.location}/3rd/pugixml/include"
IncludeDirs["rapidobj"]="%{wks.location}/3rd/rapidobj/include"
IncludeDirs["Glad"] = "%{wks.location}/3rd/Glad/include"
IncludeDirs["EnTT"] = "%{wks.location}/3rd/EnTT/include"
IncludeDirs["stbi_image"]= "%{wks.location}/3rd/stbi_image/include"

-- Dlls

DllDirs={}

-- Libs

LibDirs={}
LibDirs["GLFW"]="%{wks.location}/3rd/GLFW/lib/glfw3.lib"  



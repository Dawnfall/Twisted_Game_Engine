# Twisted Engine
Game engine with its editor that intends to focus on procedural content and its tools.

# Features

# Dependecies
 - Imgui for editor UI (submodule)
 - glfw for window creation and inputs (vcpk)
 - glad for openGL extensions
 - glm as math library
 - spdlog for logging
 - entt for ECS 

# Usage
1. Project uses submodules, so make sure to clone it recursively:
```
git clone --recurse-submodules https://github.com/yourusername/yourproject.git
```

2. Project uses vcpkg for most dependecies so you need to have it installed:
```
git clone https://github.com/microsoft/vcpkg.git
cd .\vcpkg
.\bootstrap-vcpkg.bat  # On Windows
vcpkg integrate install
```

Important to add vcpkg to your system PATH if you want it to work automatically.
from project root folder run:

```
vcpkg install
```

3. Project uses CMake to generate build files so you need to have that installed
To run cmake you have 2 options: 
- use provided batch script `GenerateCMakeSolution.bat`
- run CMake manually:
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

# ScreenShots

# Plan
-Assets:

 - model loading
 
-Graphics:

-Editor:
   --nicer UI

-Dedicated website

# Acknowledgments

 - [Cherno Hazel] Engine
 - [Benny Box](https://www.youtube.com/@thebennybox) youtube chanel for inspiring me to learn Game Engine development
 - Other game engine [Unity](TODO) / [Unreal](TODO) / [Godot](TODO)
 - ChatGpt and Github copilot for assistence

 # License

 MIT License

Copyright (c) 2025 Dawnfall

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
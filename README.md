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

# Setup Requirements

## Compiler
**Clang + MinGW-w64** - Fully portable, no Visual Studio needed!

✅ **Auto-downloads** on first build (~500MB, cached after)
✅ **Zero dependencies** - Just CMake + vcpkg  
✅ **Perfect team sync** - Everyone uses Clang 18.1.8
✅ **Cross-platform** - Windows, Linux, macOS
⏱️ **First build**: ~2-3 minutes to download
⚡ **Subsequent builds**: Instant

MinGW-w64 means no Visual Studio required—truly portable across systems!

## Usage
1. **Clone recursively** (includes submodules):
```bash
git clone --recurse-submodules https://github.com/yourusername/yourproject.git
```

2. **Install dependencies via vcpkg**:
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat  # Windows
./bootstrap-vcpkg.sh   # Linux/macOS
vcpkg integrate install
```
Add vcpkg to system PATH. Then from project root:
```bash
vcpkg install
```

3. **Set VCPKG_ROOT environment variable**:
```powershell
# Windows PowerShell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"

# Linux/macOS
export VCPKG_ROOT=/path/to/vcpkg
```

4. **Build with CMake**:
```bash
# Windows
cmake --preset clang-windows
cmake --build --preset Release-clang-windows

# Linux  
cmake --preset linux
cmake --build --preset Release-linux
```
⚠️ **First build takes 2-3 minutes** to download Clang (~500MB). Subsequent builds are instant.

## CLion Setup
1. Open project in CLion
2. **File → Settings → Build, Execution, Deployment → CMake**
3. Under "CMake Presets", select `clang-windows` (Windows) or `linux` (Linux)
4. **Build with Shift+F10** — Clang downloads automatically on first build

✓ Everything else is automatic!

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
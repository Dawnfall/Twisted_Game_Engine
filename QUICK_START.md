# Twisted Engine - Quick Start Guide

## Prerequisites
- CMake 3.28+
- vcpkg installed with `VCPKG_ROOT` environment variable set
- Ninja (optional, built into CLion)

**That's it!** Clang+MinGW downloads automatically on first build.

## First Build (3 minutes)

### On Windows
```powershell
# Set VCPKG_ROOT if not already set
$env:VCPKG_ROOT = "C:\path\to\vcpkg"

# Configure and build
cmake --preset clang-windows
cmake --build --preset Release-clang-windows
```

### On Linux
```bash
export VCPKG_ROOT=/path/to/vcpkg
cmake --preset linux
cmake --build --preset Release-linux
```

### On macOS
```bash
export VCPKG_ROOT=/path/to/vcpkg
cmake --preset linux  # same as Linux
cmake --build --preset Release-linux
```

**First build**: ~2-3 minutes (downloads Clang ~400-500MB)
**After that**: Instant builds ⚡

---

## CLion IDE Setup

1. **Open project** in CLion
2. **File → Settings → Build, Execution, Deployment → CMake**
3. Select preset:
   - **Windows**: `clang-windows` or `clang-windows-vulkan`
   - **Linux/macOS**: `linux` or `linux-vulkan`
4. **Build**: Shift+F10
5. **Run**: Shift+F10

Everything else is automatic!

---

## What's Happening Behind the Scenes

1. **CMake FetchContent** downloads LLVM/Clang binary on first configure
2. **Stored locally** in `.cmake-tools/clang-18.1.8/` (git-ignored)
3. **Subsequent builds** use cached copy (instant)
4. **Team consistency**: Everyone gets Clang 18.1.8

---

## Customization

### Use different Clang version
Edit `cmake/FetchClang.cmake`:
```cmake
set(CLANG_VERSION "19.1.0")  # change this
```

### Use system Clang instead
Edit `cmake/FetchClang.cmake`:
```cmake
set(FETCH_CLANG OFF)  # use system clang
```

### Enable Vulkan backend
Use presets ending in `-vulkan`:
```bash
cmake --preset clang-windows-vulkan
```

---

## Troubleshooting

**"CMake can't find cmake-modules"**
→ Make sure you're running from project root

**"VCPKG_ROOT not set"**
→ Set environment variable: 
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

**"Download failed"**
→ Check internet, or delete `.cmake-tools/` and try again

**"Can't find Ninja"**
→ Optional, but recommended:
```powershell
scoop install ninja  # Windows
sudo apt install ninja-build  # Linux
```

---

## Project Structure
```
Twisted_Engine/
├── Core/              # Core engine library
├── Renderer/          # Rendering backend (OpenGL/Vulkan)
├── Window/            # Window management
├── World/             # Scene/ECS system
├── Editor/            # Editor application
├── TwistedEngine/     # Main DLL
├── Assets/            # Asset processing
├── ShaderParser/      # Shader parsing
└── cmake/             # CMake modules (FetchClang, ImGui, etc.)
```

---

## Development Workflow

1. Make code changes
2. Build: `cmake --build --preset Release-clang-windows`
3. Test/run your changes
4. Commit to git

The Clang compiler is handled automatically!

---

## See Also
- [SETUP_CLANG.md](SETUP_CLANG.md) - Detailed Clang documentation
- [README.md](README.md) - Project overview
- [CMakePresets.json](CMakePresets.json) - Available build presets

# Twisted Engine - Clang Setup Guide

This project uses **Clang 18.1.8 with MinGW-w64** for maximum portability and zero external dependencies.

## Quick Start (Automatic)

**No manual installation needed!** CMake automatically downloads Clang+MinGW on first build:

```bash
# Windows
cmake --preset clang-windows
cmake --build --preset Release-clang-windows

# Linux  
cmake --preset linux
cmake --build --preset Release-linux
```

⏱️ **First build**: ~2-3 minutes to download (~500MB)
⚡ **After that**: Instant (cached locally)

### Why MinGW-w64?

| Feature | Clang+MinGW | Clang-cl |
|---------|-----------|----------|
| **Setup** | Auto-download | Needs Visual Studio |
| **Dependencies** | None | Visual Studio required |
| **Portability** | ✅ Excellent | Limited to VS users |
| **Performance** | ✅ Fast | Fast |
| **C++23 Support** | ✅ Full | Full |

**MinGW-w64 is the portable choice.** It works on any Windows system without Visual Studio.

---

## Manual Setup (Optional)

If you prefer to use your system Clang or have existing installation, follow below. Otherwise, skip to CLion setup.

## Windows Setup

### Option 1: Scoop (Recommended - Easiest)
```powershell
scoop install llvm
```
This automatically adds Clang to your PATH.

### Option 2: Chocolatey
```powershell
choco install llvm
```

### Option 3: Manual Download
1. Download LLVM from https://releases.llvm.org/
2. Install with "Add LLVM to PATH" option checked
3. Verify installation:
   ```powershell
   clang-cl --version
   clang++ --version
   ```

### Option 4: Visual Studio (Built-in)
If you have Visual Studio 2026+ with C++ support, Clang is bundled:
- Install via Visual Studio Installer → Individual Components → "C++ Clang tools for Windows"

## Linux Setup

Most modern Linux distributions have Clang available:

```bash
# Ubuntu/Debian
sudo apt-get install clang clang++

# Fedora
sudo dnf install clang

# Arch
sudo pacman -S clang

# Verify
clang --version
clang++ --version
```

For newer Clang versions, use:
```bash
# Ubuntu (via LLVM script)
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
```

## macOS Setup

```bash
# Via Homebrew
brew install llvm

# Via Xcode Command Line Tools (includes Clang)
xcode-select --install
```

## Building with Clang in CLion

### Step 1: Select Preset
1. Open project in CLion
2. Go to **File → Settings → Build, Execution, Deployment → CMake**
3. Under "CMake Presets", select:
   - **Windows**: `clang-windows` (OpenGL) or `clang-windows-vulkan`
   - **Linux**: `linux` (OpenGL) or `linux-vulkan`
   - **macOS**: Use standard Clang via system toolchain

### Step 2: Configure Build
1. Click **File → Settings → Build, Execution, Deployment → Toolchains**
2. Select the Clang preset
3. Verify "C Compiler" and "C++ Compiler" point to Clang

### Step 3: Build & Run
Press **Shift + F10** to build and run.

## Building from Command Line

### Windows
```bash
cmake --preset clang-windows
cmake --build --preset Release-clang-windows
```

### Linux
```bash
cmake --preset linux
cmake --build --preset Release-linux
```

## Troubleshooting

### "clang-cl not found"
- Ensure Clang is installed and in PATH
- Add Clang to PATH manually: `$env:PATH += ";C:\Program Files\LLVM\bin"`
- Restart your terminal after installing Clang

### "Ninja not found"
```powershell
# Windows
scoop install ninja
# or
choco install ninja
```

### Compilation fails with C++ standard errors
- Ensure you're using Clang 15+ (supports C++23)
- Check: `clang++ --version`

### Performance issues
- Use Release build: `--config Release`
- Enable LTO in CMakeLists.txt if needed

## CMake Integration

The presets automatically detect and use Clang when:
1. `clang-cl` (Windows) or `clang++` (Linux) are in PATH
2. Environment variable `CLANG_ROOT` is set (advanced)
3. Visual Studio's bundled Clang is available as fallback

No manual compiler paths needed!

## Auto-Download Details (FetchContent)

The project uses CMake's **FetchContent** module to automatically download Clang from LLVM releases:

### How It Works
1. First CMake configure: Downloads pre-built LLVM/Clang binary
2. Extraction: Stored in `.cmake-tools/clang-18.1.8/` (git-ignored)
3. Subsequent builds: Uses cached copy (instant)

### Configuration
Edit CMakeLists.txt to customize:

```cmake
# Change Clang version
set(CLANG_VERSION "18.1.8" CACHE STRING "LLVM/Clang version")

# Disable auto-download, use system Clang
set(FETCH_CLANG OFF CACHE BOOL "Auto-download Clang")
```

### What Gets Downloaded
- **Windows**: LLVM 18.1.8 x86_64 (~500MB)
- **Linux**: LLVM 18.1.8 x86_64 (~400MB) 
- **macOS**: LLVM 18.1.8 x86_64 or ARM64 (~450MB)

### Storage
- Location: `build/.cmake-tools/clang-18.1.8/`
- Not committed to git (in `.gitignore`)
- Safe to delete and re-download

### Troubleshooting Auto-Download

**"Failed to download"**
- Check internet connection
- Try manual download from: https://github.com/llvm/llvm-project/releases
- Or set `FETCH_CLANG=OFF` and install system Clang

**"Slow download"**
- First build only, ~2-3 minutes depending on connection
- Consider cloning via mobile hotspot or downloading offline

**"Different version needed"**
- Edit `set(CLANG_VERSION "X.X.X")` in `cmake/FetchClang.cmake`
- Delete `.cmake-tools/` folder
- Re-run configure

## Advantages of Clang

✅ Better C++23 support than MSVC
✅ Cross-platform compatibility (Windows, Linux, macOS)
✅ Faster compilation in parallel builds
✅ Better error messages
✅ Smaller binary sizes
✅ **Team consistency** - same Clang version for all developers

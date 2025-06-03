#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <vector>
#include <string>

#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <thread>
#include <ctime>
#include <chrono>
#include <functional>
#include <algorithm>
#include <fstream>
#include <regex>

#include <cassert>
#include <type_traits>

#include <sys/stat.h>
#include <Windows.h>

//imgui
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

//glm
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtx/string_cast.hpp>

//From file dialog
#ifdef _WIN32
#define NOMINMAX

#include <shellapi.h>
#include <lmcons.h>
#pragma comment(lib, "Shell32.lib")
#else
#include <unistd.h>
#include <pwd.h>
#endif

namespace fs = std::filesystem;
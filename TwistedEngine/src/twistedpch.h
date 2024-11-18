#pragma once

#define NOMINMAX //to be able to include windows
#include <windows.h>

#include <memory>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <algorithm>
#include <fstream>
#include <regex>

#include <cassert>
#include <type_traits>

//entt
#include <EnTT/entt.hpp>

//spdlog
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//glfw
#include <GLFW/glfw3.h>

//glad
#include <glad/glad.h>

//glm
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtx/string_cast.hpp>

//stbi
#include <stbi_image/stb_image.h>

#ifdef TWISTED_ENGINE

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//pugixml
#include <pugixml/pugixml.hpp>

#endif

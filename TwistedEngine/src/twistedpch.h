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

//glm
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtx/string_cast.hpp>



#ifdef TWISTED_ENGINE

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#endif

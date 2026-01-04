#pragma once

#include "Shaders/Debug_Normal_Shader.h"
#include "Shaders/Debug_UV_Shader.h"
#include "Shaders/Debug_Normal_Shader.h"
#include "Shaders/Default_Shader.h"
#include "Shaders/Light_Shader.h"
#include "Shaders/Simple_Shader.h"

#include "Meshes/TriangleMesh.h"
#include "Meshes/QuadMesh.h"
#include "Meshes/CubeMesh.h"
#include "Meshes/SphericalMesh.h"

#include <functional>
#include <vector>

namespace Twisted::Collections
{
	class BuiltInRegistry
	{
	public:
		inline static BuiltInRegistry& GetInstance()
		{
			static BuiltInRegistry s_instance;
			return s_instance;
		}

		BuiltInRegistry(const BuiltInRegistry& other) = delete;
		BuiltInRegistry(BuiltInRegistry&& other) = delete;
		BuiltInRegistry& operator=(const BuiltInRegistry& other) = delete;
		BuiltInRegistry& operator=(BuiltInRegistry&& other) = delete;

		inline void ImportAll()
		{
			ImportTriangleMesh();
			ImportQuadMesh();
			ImportCubeMesh();
			ImportSphericalSphereMesh();

			ImportDefaultShader();
			ImportDebugUVsShader();
			ImportDebugNormalsShader();
			ImportSimpleShader();
			ImportLightsShader();
		}

	private:
		BuiltInRegistry() = default;

		std::vector<std::function<void(void)>> m_registeredBuiltInMethods;
	};
}


/* UUIDs
{ 0xF12D8C33B7A4509AULL, 0x62A4E9D0C8B15F23ULL }
{ 0x9D77A4E1C3395B20ULL, 0xFEB218D4A6C7093DULL }
{ 0x4B98F6C2D1157A9FULL, 0x83D44AA129E0FE31ULL }
{ 0xC8E2B17495AF03D2ULL, 0x1D76C5E39AA4E870ULL }
{ 0xA1F4DE62B38C9045ULL, 0x7F23C8B1D4E96A8CULL }
{ 0x3CE7A1B59D0F72A4ULL, 0xE4B99873CDA25119ULL }
{ 0xD2A4587CB90E13EFULL, 0x6AFC0471E8B3D95CULL }
*/
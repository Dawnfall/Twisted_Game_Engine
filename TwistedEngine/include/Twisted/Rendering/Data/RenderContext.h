#pragma once
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Data/ClearParams.h"

#include <vector>

namespace Twisted
{
	struct TWISTED_API CameraData
	{
		Mat4x4f viewMatrix;
		Mat4x4f projectionMatrix;
		Framebuffer* framebuffer = nullptr;
		ClearParams clearParams;
	};

	struct TWISTED_API LightData
	{
		std::vector<GPUDirLight>   dirLights;
		std::vector<GPUPointLight> pointLights;
		std::vector<GPUSpotLight>  spotLights;
	};

	struct TWISTED_API ModelData
	{
		Mat4x4f modelMatrix;
		Mesh* mesh;
		Material* material;
	};

	struct TWISTED_API RenderContext
	{
		std::vector<ModelData> modelDatas;
		std::vector<CameraData> camDatas;
		LightData lightData;
	};
}

#pragma once
#include "Utils/GlmUtils.h"
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
		Framebuffer* framebuffer=nullptr;
		ClearParams clearParams;
	};

	struct TWISTED_API DirLightData
	{
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float _padding[3];
	};

	struct TWISTED_API PointLightData
	{
		Vec4f position;
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float radius;
		float _padding[2];
	};
	struct TWISTED_API SpotLightData
	{
		Vec4f position;
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float radius;
		float inner;
		float outer;
	};

	struct TWISTED_API LightData
	{
		std::vector<DirLightData> dirLights;
		std::vector<PointLightData> pointLights;
		std::vector<SpotLightData> spotLights;
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
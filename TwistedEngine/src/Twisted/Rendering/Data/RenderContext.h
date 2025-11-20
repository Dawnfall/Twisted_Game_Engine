#pragma once
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/FrameBuffer.h"

#include <vector>
namespace Twisted
{
	struct CameraData
	{
		Mat4x4f viewMatrix;
		Mat4x4f projectionMatrix;
		FrameBuffer* framebuffer;
		ClearParams clearParams;
	};

	struct DirLightData
	{
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float _padding[3];
	};
	struct PointLightData
	{
		Vec4f position;
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float radius;
		float _padding[2];
	};
	struct SpotLightData
	{
		Vec4f position;
		Vec4f direction;
		Vec4f lightColor;
		float intensity;
		float radius;
		float inner;
		float outer;
	};

	struct LightData
	{
		std::vector<DirLightData> dirLights;
		std::vector<PointLightData> pointLights;
		std::vector<SpotLightData> spotLights;
	};

	struct ModelData
	{
		Mat4x4f modelMatrix;
		Mesh* mesh;
		Material* material;
	};

	struct RenderContext
	{
		std::vector<ModelData> modelDatas;
		std::vector<CameraData> camDatas;
		LightData lightData;
	};



}
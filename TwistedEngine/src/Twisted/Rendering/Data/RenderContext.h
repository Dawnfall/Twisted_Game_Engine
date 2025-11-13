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

	};



}
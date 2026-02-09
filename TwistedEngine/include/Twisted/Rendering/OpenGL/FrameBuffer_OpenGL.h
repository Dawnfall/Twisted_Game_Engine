#ifndef TWISTED_D3D
#pragma once
#include "AppCore.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include <Utils/GlmUtils.h>
#include "Utils/WPtr.h"


namespace Twisted::GL
{
	class FrameBuffer_OpenGL
	{
	public:
		FrameBuffer_OpenGL();
		~FrameBuffer_OpenGL();

		bool IsValid()const
		{
			return Id != 0 && Tex && Tex->IsValid();
		}

		void Bind();
		void UnBind();

		void Update();

		unsigned int Id = 0;
		unsigned int Rbo = 0;
		Vec2i Size{ 0,0 };
		bool IsDirty = true;
		URef<Texture_OpenGL> Tex;
	private:
	};

	void TWISTED_API Blit(FrameBuffer_OpenGL& framebuffer, unsigned int destID);
	void TWISTED_API ClearBuffer(const ClearParams& clearParams); //assumes bound buffer
}

#endif
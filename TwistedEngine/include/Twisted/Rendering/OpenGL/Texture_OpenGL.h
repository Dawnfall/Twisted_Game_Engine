#ifndef TWISTED_D3D
#pragma once

#include "Twisted/Rendering/Texture.h"
#include <glad/glad.h>

namespace Twisted::GL
{
	class TWISTED_API Texture_OpenGL
	{
	public:
		bool IsValid()const { return TexID != 0; }
		unsigned int TexID = 0;

		void Resize(const Vec2i& newSize);
		void Bind(unsigned int slot);
		void UnBind();

		void Clear();

		TextureParams Params;
		int Width = 0;
		int Height = 0;
		int Channels = 0;
	private:
	};

	GLenum TWISTED_API WrapToGL(TextureWrap wrap);
	GLenum TWISTED_API MagFilterToGL(TextureMagFilter magFilter);
	GLenum TWISTED_API MinFilterToGL(TextureMinFilter minFilter);

	void TWISTED_API SetMagFilter(Texture_OpenGL tex, TextureMagFilter magFilter);
	void TWISTED_API SetParams(Texture_OpenGL& tex, const TextureParams& params);
	void TWISTED_API SetMinFilter(Texture_OpenGL& tex, TextureMinFilter minFilter);
	void TWISTED_API SetWrapType(Texture_OpenGL& tex, TextureWrap wrapType);

	void TWISTED_API ApplyParams(const TextureParams& params); //assumes bound and valid


}

#endif
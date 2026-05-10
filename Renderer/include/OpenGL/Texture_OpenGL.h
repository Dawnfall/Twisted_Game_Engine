#ifdef TWISTED_BACKEND_OPENGL
#pragma once

#include "Texture.h"
#include <glad/glad.h>

namespace Twisted
{
	struct TextureBackend
	{
		bool IsValid()const { return TexID != 0; }
		GLuint TexID = 0;
	};
}
namespace Twisted::GL
{
	void TWISTED_API Bind(GLuint texID, GLenum slot);
	void TWISTED_API UnBind();


	GLenum TWISTED_API WrapToGL(TextureWrap wrap);
	GLenum TWISTED_API MagFilterToGL(TextureMagFilter magFilter);
	GLenum TWISTED_API MinFilterToGL(TextureMinFilter minFilter);
	GLenum TWISTED_API ToGLInternalFormat(TextureFormat texFormat);
	void ToGLExternalFormatAndType(TextureFormat fmt, GLenum& outFormat, GLenum& outType);
	int CalcMipCount2D(int w, int h);


	void TWISTED_API SetMagFilter(GLuint tex, TextureMagFilter& magFilter);
	void TWISTED_API SetParams(GLuint tex, const TextureParams& params);
	void TWISTED_API SetMinFilter(GLuint tex, TextureMinFilter minFilter);
	void TWISTED_API SetWrapType(GLuint tex, TextureWrap wrapType);

	void TWISTED_API ApplyParams(const TextureParams& params); //assumes bound and valid
}

#endif
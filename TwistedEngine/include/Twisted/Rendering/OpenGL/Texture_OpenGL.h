#include "Twisted/Rendering/Texture.h"

#include <glad/glad.h>

namespace Twisted::Texture_GL
{
	GLenum TWISTED_API WrapToGL(TextureWrap wrap);
	GLenum TWISTED_API MagFilterToGL(TextureMagFilter magFilter);
	GLenum TWISTED_API MinFilterToGL(TextureMinFilter minFilter);

	void TWISTED_API Clear(Texture& tex);

	void TWISTED_API SetData(Texture& tex, TextureData& data);

	void TWISTED_API Resize(Texture& tex, const Vec2i& newSize);

	void TWISTED_API SetMagFilter(Texture tex, TextureMagFilter magFilter);

	void TWISTED_API SetParams(Texture& tex, const TextureParams& params);

	void TWISTED_API SetMinFilter(Texture& tex, TextureMinFilter minFilter);

	void TWISTED_API SetWrapType(Texture& tex, TextureWrap wrapType);

	void TWISTED_API ApplyParams(const TextureParams& params); //assumes bound and valid

	void TWISTED_API Bind(const Texture& tex, unsigned int slot);

	void TWISTED_API UnBind();


}
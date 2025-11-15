#include "Twisted/Rendering/Texture.h"

#include <glad/glad.h>

namespace Twisted::Texture_GL
{
	GLenum WrapToGL(TextureWrap wrap);
	GLenum MagFilterToGL(TextureMagFilter magFilter);
	GLenum MinFilterToGL(TextureMinFilter minFilter);

	void Clear(Texture& tex);

	void SetData(Texture& tex, TextureData& data);

	void Resize(Texture& tex, const Vec2i& newSize);

	void SetMagFilter(Texture tex, TextureMagFilter magFilter);

	void SetParams(Texture& tex, const TextureParams& params);

	void SetMinFilter(Texture& tex, TextureMinFilter minFilter);

	void SetWrapType(Texture& tex, TextureWrap wrapType);

	void ApplyParams(const TextureParams& params); //assumes bound and valid

	void Bind(const Texture& tex, unsigned int slot);

	void UnBind();


}
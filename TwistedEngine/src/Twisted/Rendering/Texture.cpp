#include "twistedpch.h"
#include "Texture.h"
#include "RenderingAPI.h"

namespace Twisted
{
	Texture::~Texture()
	{
		RenderAPI::DestroyTexture(this);
	}
}
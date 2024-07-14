#include "RenderCore.h"

namespace Dawn
{
	bool RenderCore::Init()
	{
		if (!glfwInit())
			return false;

		glfwSetErrorCallback(RenderCore::ErrorCallback);
		DAWN_INFO("RenderCore Init success!");

		return true;
	}

	void RenderCore::Terminate()
	{
		glfwTerminate();
	}

	void RenderCore::ErrorCallback(int code, const char* description)
	{
		DAWN_ERROR(description);
	}
}
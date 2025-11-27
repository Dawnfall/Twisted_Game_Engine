#include "Render_OpenGL.h"

#include <glad/glad.h>

namespace Twisted::Render_GL
{
	void Init()
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void EnableDepthTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthMask(doTest ? GL_TRUE : GL_FALSE); //TODO... maybe separate function
	}

	void EnableStencilTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		glStencilMask(doTest ? GL_TRUE : GL_FALSE); //TODO... maybe separate function
	}






}
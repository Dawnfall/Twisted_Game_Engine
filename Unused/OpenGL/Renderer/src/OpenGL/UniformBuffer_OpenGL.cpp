#ifdef TWISTED_BACKEND_OPENGL

#include <glad/glad.h>
#include "UniformBuffer.h"
#include "Data/ShaderData.h"
namespace Twisted
{


	struct UniformBuffer_OpenGL
	{
		GLuint bufferID;
	};

	[[maybe_unused]] static GLuint CreateUBO(GLsizeiptr bufferSize)
	{
		// ---------------------------
		// Create UBO buffer
		// ---------------------------

		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return id;
	}
}

namespace Twisted::GL
{
	//void Bind(GLuint shader, UniformBlockDesc blockInfo)
	//{
	//	// ---------------------------
	//	// Assign binding point
	//	// ---------------------------
	//	//glUniformBlockBinding(shader, blockInfo.glBlockIndex, blockInfo.glBinding);

	//	// ---------------------------
	//	// Bind UBO object to binding point
	//	// ---------------------------
	//	//glBindBufferBase(GL_UNIFORM_BUFFER, blockInfo.glBinding, blockInfo.BufferID);
	//}

}

#endif

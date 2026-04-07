#ifndef TWISTED_D3D

#pragma once

#include <glad/glad.h>

namespace Twisted::GL
{
	template<typename T>
	struct UniformBuffer
	{
		GLuint id = 0;
		GLuint bindingPoint = 0;

		// Auto-discovers the binding point from T::kBindingPoint (requires ShaderBuffer<N> base).
		void Create() requires requires { T::kBindingPoint; }
		{
			Create(static_cast<GLuint>(T::kBindingPoint));
		}

		void Create(GLuint binding)
		{
			bindingPoint = binding;
			glGenBuffers(1, &id);
			glBindBuffer(GL_UNIFORM_BUFFER, id);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);
		}

		void Upload(const T& data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, id);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void Destroy()
		{
			if (id)
			{
				glDeleteBuffers(1, &id);
				id = 0;
			}
		}
	};
}

#endif

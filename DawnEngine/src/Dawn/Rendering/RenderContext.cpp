#include "RenderContext.h"

#include <glad/glad.h>

namespace Dawn
{
	void RenderContext::Init(Window* window)
	{
		//TODO: prolly init in context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{

		}

		glViewport(0, 0, window->m_windowData.Width, window->m_windowData.Height); //bot left, width,height
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	}

	void RenderContext::Render(Mesh& mesh, Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < mesh.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = mesh.textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			shader.setInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

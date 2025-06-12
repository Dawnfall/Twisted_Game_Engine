#include "Twisted/Rendering/Mesh.h"
#include <glad/glad.h>

namespace Twisted::Render
{
	Mesh::Mesh(const MeshData& meshData) :
		IndicesSize(meshData.Vertices.size() * sizeof(Vertex))
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, meshData.Vertices.size() * sizeof(Vertex), &meshData.Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.Indices.size() * sizeof(unsigned int), &meshData.Indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

		glBindVertexArray(0);
	}
	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	void Mesh::Bind()const
	{
		glBindVertexArray(VAO);
	}
	void Mesh::UnBind()const
	{
		glBindVertexArray(0);
	}
	void Mesh::Render()const
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)IndicesSize, GL_UNSIGNED_INT, 0);
	}

}

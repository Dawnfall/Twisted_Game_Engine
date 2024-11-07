#pragma once

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Resources/ShaderData.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"

#include "Collections/Color.h"
#include "Collections/Geometry.h"

namespace Twisted::RenderAPI
{
	bool InitGLFW();
	bool InitOpenGL(GLADloadproc loadproc);
	void Terminate();

	void Render(const CRenderer& renderer);
	std::shared_ptr<Mesh> CreateMesh(MeshData meshData);

	void ClearWindow(Colors::Color color);
	void SetViewPort(float width, float height);
	void SetVsync(int deltaFrames);

	//****************
	// Buffers

	void ClearBuffers(bool doClearColor, Colors::Color clearColor, bool doClearDepth, bool doClearStencil);
	void SetEnableDepthTest(bool doEnable);
	void SetEnableDepthWrite(bool doEnable);

	void SetDepthTestFunc();//TODO...

	void SetEnableStencilTest(bool doEnable);



	//*****************************
	// Texture

	std::shared_ptr<Texture> LoadTexture(const std::string& name, int width, int height, unsigned char* data);
	void BindTexture(std::shared_ptr<Texture> texture);


	//	static void BindTexture(int id);
	void glfwErrorCallback(int code, const char* description);
	void openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};
  // detect uniforms different way

  /*void Detect()
  {
	  GLint numActiveAttribs = 0;
	  GLint numActiveUniforms = 0;
	  glGetProgramInterfaceiv(ID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	  glGetProgramInterfaceiv(ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	  std::vector<GLchar> nameData(256);
	  std::vector<GLenum> properties;
	  properties.push_back(GL_NAME_LENGTH);
	  properties.push_back(GL_TYPE);
	  properties.push_back(GL_ARRAY_SIZE);

	  std::vector<GLint> values(properties.size());
	  for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
	  {
		  glGetProgramResourceiv(ID, GL_PROGRAM_INPUT, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);

		  nameData.resize(values[0]);
		  glGetProgramResourceName(ID, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
		  std::string name((char*)&nameData[0], nameData.size() - 1);
	  }
  }*/
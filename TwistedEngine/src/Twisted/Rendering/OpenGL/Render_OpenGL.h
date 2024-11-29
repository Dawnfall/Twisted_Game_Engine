#pragma once

#include "Assets/MeshData.h"
#include "Assets/TextureData.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"
#include "Assets/ShaderData.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"

#include "Collections/Color.h"
#include "Collections/Geometry.h"

#include "AppCore.h"

namespace Twisted::RenderAPI
{
	bool TWISTED_API InitGLFW();
	bool TWISTED_API InitOpenGL(GLADloadproc loadproc);
	void TWISTED_API Terminate();

	void TWISTED_API Render(const CRenderer& renderer);
	SRef<Mesh> TWISTED_API CreateMesh(SRef<MeshData> meshData);

	void TWISTED_API ClearWindow(Colors::Color color);
	void TWISTED_API SetViewPort(float width, float height);
	void TWISTED_API SetVsync(int deltaFrames);

	//****************
	// Buffers

	void TWISTED_API ClearBuffers(bool doClearColor, Colors::Color clearColor, bool doClearDepth, bool doClearStencil);
	void TWISTED_API SetEnableDepthTest(bool doEnable);
	void TWISTED_API SetEnableDepthWrite(bool doEnable);

	void TWISTED_API SetDepthTestFunc();//TODO...

	void TWISTED_API SetEnableStencilTest(bool doEnable);



	//*****************************
	// Texture

	SRef<Texture> TWISTED_API LoadTexture(SRef<TextureData> texData);
	void TWISTED_API BindTexture(SRef<Texture> texture);
	void TWISTED_API DestroyTexture(Texture* texture);

	//	static void BindTexture(int id);
	void TWISTED_API glfwErrorCallback(int code, const char* description);
	void TWISTED_API openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
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
#pragma once
#include "Twisted/Rendering/Shader.h"
#include "Assets/ShaderData.h"
#include "Twisted/Rendering/Material.h"

#include "Collections/Color.h"
#include "Collections/Geometry.h"

namespace Twisted::ShaderAPI
{
	//	//************
//	// Shader

	SRef<Shader> CreateShader(SRef<ShaderData> shaderData);
	void UnCompileShader(Shader& shader);

	void SetUniforms(const SRef<Material>& material);

	// Utility uniform functions
	void setBool(GLint locationID, bool value);
	void setInt(GLint locationID, int value);
	void setFloat(GLint locationID, float value);
	void setFloat2(GLint locationID, const Vec2f& value);
	void setFloat3(GLint locationID, const Vec3f& value);
	void setFloat4(GLint locationID, const Vec4f& value);
	void setMat4(GLint locationID, const Mat4x4f& value);

	GLuint CompileShader(GLenum shaderType, const char* shaderName, const char* shaderCode);
	GLuint CompileProgram(GLuint vertexID, GLuint fragmentID);
	std::vector<ShaderUniformVar> DetectUniformVars(GLuint programID);



}
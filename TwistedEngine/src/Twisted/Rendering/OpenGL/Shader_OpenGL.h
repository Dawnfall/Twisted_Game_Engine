#pragma once 

#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>
#include <vector>

namespace Twisted::Shader_GL
{
	ShaderVarType FromGLShaderType(GLenum glShaderVarType);
	GLenum ToGlShaderType(ShaderVarType shaderVarType);
	bool ValidateShader(GLuint id, const std::string& name, GLenum shaderType);
	bool ValidateProgram(GLuint id, const std::string& name);
	[[nodiscard]] GLuint CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
	[[nodiscard]] GLuint LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID);
	std::vector<ShaderUniformBlock> detectShaderUniformBlocks(GLuint shaderProgram);
	std::vector<ShaderUniformVar> detectShaderUniformVars(GLuint shaderProgram);

	void SetData(Shader& shader, const ShaderData& shaderData);
	void Clear(Shader& shader);
	void Bind(const Shader& shader);
	void UnBind();

	void SetVar(int locationID, bool value);
	void SetVar(int locationID, int value);
	void SetVar(int locationID, float value);
	void SetVar(int locationID, double value);
	void SetVar(int locationID, unsigned int value);
	void SetVar(int locationID, const Vec4d& value);
	void SetVar(int locationID, const Vec3d& value);
	void SetVar(int locationID, const Vec2d& value);
	void SetVar(int locationID, const Vec4f& value);
	void SetVar(int locationID, const Vec3f& value);
	void SetVar(int locationID, const Vec2f& value);
	void SetVar(int locationID, const Vec4i& value);
	void SetVar(int locationID, const Vec3i& value);
	void SetVar(int locationID, const Vec2i& value);
	void SetVar(int locationID, const Mat4x4f& value);
	void SetVar(int locationID, const Mat3x3f& value);
	void SetVar(int locationID, const Mat2x2f& value);
	void SetVar(int locationID, const Mat4x4d& value);
	void SetVar(int locationID, const Mat3x3d& value);
	void SetVar(int locationID, const Mat2x2d& value);
	void SetTex(int locationID, int unit, unsigned int texID);
	void SetBuffer(int bufferID, const void* data, size_t size);
}
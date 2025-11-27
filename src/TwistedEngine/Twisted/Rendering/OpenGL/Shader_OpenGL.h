#pragma once 

#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>
#include <vector>

namespace Twisted::Shader_GL
{
	ShaderVarType TWISTED_API FromGLShaderType(GLenum glShaderVarType);
	GLenum TWISTED_API ToGlShaderType(ShaderVarType shaderVarType);
	bool TWISTED_API ValidateShader(GLuint id, const std::string& name, GLenum shaderType);
	bool TWISTED_API ValidateProgram(GLuint id, const std::string& name);
	[[nodiscard]] GLuint TWISTED_API CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
	[[nodiscard]] GLuint TWISTED_API LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID);
	std::vector<ShaderUniformBlock> TWISTED_API detectShaderUniformBlocks(GLuint shaderProgram);
	std::vector<ShaderUniformVar> TWISTED_API detectShaderUniformVars(GLuint shaderProgram);

	void TWISTED_API SetData(Shader& shader, const ShaderData& shaderData);
	void TWISTED_API Clear(Shader& shader);
	void TWISTED_API Bind(const Shader& shader);
	void TWISTED_API UnBind();

	void TWISTED_API SetVar(int locationID, bool value);
	void TWISTED_API SetVar(int locationID, int value);
	void TWISTED_API SetVar(int locationID, float value);
	void TWISTED_API SetVar(int locationID, double value);
	void TWISTED_API SetVar(int locationID, unsigned int value);
	void TWISTED_API SetVar(int locationID, const Vec4d& value);
	void TWISTED_API SetVar(int locationID, const Vec3d& value);
	void TWISTED_API SetVar(int locationID, const Vec2d& value);
	void TWISTED_API SetVar(int locationID, const Vec4f& value);
	void TWISTED_API SetVar(int locationID, const Vec3f& value);
	void TWISTED_API SetVar(int locationID, const Vec2f& value);
	void TWISTED_API SetVar(int locationID, const Vec4i& value);
	void TWISTED_API SetVar(int locationID, const Vec3i& value);
	void TWISTED_API SetVar(int locationID, const Vec2i& value);
	void TWISTED_API SetVar(int locationID, const Mat4x4f& value);
	void TWISTED_API SetVar(int locationID, const Mat3x3f& value);
	void TWISTED_API SetVar(int locationID, const Mat2x2f& value);
	void TWISTED_API SetVar(int locationID, const Mat4x4d& value);
	void TWISTED_API SetVar(int locationID, const Mat3x3d& value);
	void TWISTED_API SetVar(int locationID, const Mat2x2d& value);
	void TWISTED_API SetTex(int locationID, int unit, unsigned int texID);
	void TWISTED_API SetBuffer(const std::string& name, const Shader& shader, const void* data);
}
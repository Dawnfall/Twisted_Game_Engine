#ifndef TWISTED_D3D

#pragma once 
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Data/ShaderData.h"

#include <glad/glad.h>
#include <vector>

namespace Twisted::GL
{
	struct TWISTED_API ShaderUniformVar
	{
		std::string Name = "";
		Twisted::ShaderVarType Type = Twisted::ShaderVarType::UNKNOWN;
		int TextureUnit = 0;
		int UniformID = 0;
	};

	struct TWISTED_API ShaderUniformBlock
	{
		std::string Name = "";
		unsigned int Index = 0;       // block index inside the shader
		unsigned int Binding = 0;     // binding point we assign
		unsigned int BufferID = 0;    // the generated UBO
		int Size = 0;       // block size in bytes
	};

	class TWISTED_API Shader_OpenGL
	{
	public:
		bool IsValid()const { return ProgramID != 0; }
		void Clear();

		void Bind()const;

		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderUniformBlock> UniformBlocks;
		GLuint ProgramID = 0;
	};

	ShaderVarType TWISTED_API FromGLShaderType(GLenum glShaderVarType);
	GLenum TWISTED_API ToGlShaderType(ShaderVarType shaderVarType);
	bool TWISTED_API ValidateShader(GLuint id, const std::string& name, GLenum shaderType);
	bool TWISTED_API ValidateProgram(GLuint id, const std::string& name);
	[[nodiscard]] GLuint TWISTED_API CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
	[[nodiscard]] GLuint TWISTED_API LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID);
	std::vector<ShaderUniformBlock> TWISTED_API detectShaderUniformBlocks(GLuint shaderProgram);
	std::vector<ShaderUniformVar> TWISTED_API detectShaderUniformVars(GLuint shaderProgram);


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
	void TWISTED_API SetBuffer(const std::string& name, const Shader_OpenGL& shader, const void* data);
}

#endif

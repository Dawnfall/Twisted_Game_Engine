#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/TObject.h"
#include "Utils/GlmUtils.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace Twisted
{
	class Material;

	enum class TWISTED_API ShaderVarType : uint8_t
	{
		BOOL, UNSIGNED_INT, INT, FLOAT, DOUBLE,
		VEC4_F, VEC3_F, VEC2_F,
		VEC4_D, VEC3_D, VEC2_D,
		VEC4_I, VEC3_I, VEC2_I,
		VEC4_U, VEC3_U, VEC2_U,
		MAT4x4_F, MAT3x3_F, MAT2x2_F,
		MAT4x4_D, MAT3x3_D, MAT2x2_D,
		SAMPLER2D,
		UNKNOWN
	};

	struct ShaderUniformVar
	{
		std::string Name = "";
		ShaderVarType Type = ShaderVarType::UNKNOWN;
		int TextureUnit = 0;
		int UniformID = 0;
	};

	struct ShaderData
	{
		std::string VertShader;
		std::string FragShader;
		std::string GeoShader;
	};

	template<typename T>
	struct TWISTED_API ShaderValue
	{
		int ID = -1;
		T value = T();
	};

	class TWISTED_API Shader :public TObject
	{
	public:

		Shader() = default;
		Shader(const ShaderData& shaderData) :TObject()
		{
			Init(shaderData);
		}
		~Shader()
		{
			Clear();
		}

		void Clear();
		void Init(const ShaderData& shaderData);

		bool IsValid()const { return m_programID != 0; }

		void Bind()const;
		void UnBind()const;

		unsigned int GetProgramID()const { return m_programID; }
		const std::vector<ShaderUniformVar>& GetUniforms()const { return m_uniforms; }

		void SetVar(int locationID, bool value)const;

		void SetVar(int locationID, int value)const;
		void SetVar(int locationID, float value)const;
		void SetVar(int locationID, double value)const;
		void SetVar(int locationID, unsigned int value)const;

		void SetVar(int locationID, const Vec4d& value)const;
		void SetVar(int locationID, const Vec3d& value)const;
		void SetVar(int locationID, const Vec2d& value)const;

		void SetVar(int locationID, const Vec4f& value)const;
		void SetVar(int locationID, const Vec3f& value)const;
		void SetVar(int locationID, const Vec2f& value)const;

		void SetVar(int locationID, const Vec4i& value)const;
		void SetVar(int locationID, const Vec3i& value)const;
		void SetVar(int locationID, const Vec2i& value)const;

		void SetVar(int locationID, const Mat4x4f& value)const;
		void SetVar(int locationID, const Mat3x3f& value)const;
		void SetVar(int locationID, const Mat2x2f& value)const;

		void SetVar(int locationID, const Mat4x4d& value)const;
		void SetVar(int locationID, const Mat3x3d& value)const;
		void SetVar(int locationID, const Mat2x2d& value)const;

		void SetTex(int locationID, int unit, unsigned int texID)const;

	private:

		void DetectUniformVars();

		unsigned int m_programID = 0;
		std::vector<ShaderUniformVar> m_uniforms;
	};
}


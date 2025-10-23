#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Data/Color.h"

#include "Debug/Logger.h"
#include <unordered_map>
#include <string>
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Rendering/Shader.h"
#include "Utils/WPtr.h"


//TODO: only set dirty
//TODO: set as single buffer
//TODO: dont send invalid? 

namespace Twisted
{
	struct MaterialData
	{
		std::unordered_map<std::string, unsigned int> UnsignedInts;
		std::unordered_map<std::string, bool> Bools;
		std::unordered_map<std::string, int> Ints;
		std::unordered_map<std::string, float> Floats;
		std::unordered_map<std::string, double> Doubles;
		std::unordered_map<std::string, Vec4f> Vec4fs;
		std::unordered_map<std::string, Vec3f> Vec3fs;
		std::unordered_map<std::string, Vec2f> Vec2fs;
		std::unordered_map<std::string, Vec4d> Vec4ds;
		std::unordered_map<std::string, Vec3d> Vec3ds;
		std::unordered_map<std::string, Vec2d> Vec2ds;
		std::unordered_map<std::string, Vec4i> Vec4is;
		std::unordered_map<std::string, Vec3i> Vec3is;
		std::unordered_map<std::string, Vec2i> Vec2is;
		std::unordered_map<std::string, Mat4x4f> Mat4x4fs;
		std::unordered_map<std::string, Mat3x3f> Mat3x3fs;
		std::unordered_map<std::string, Mat2x2f> Mat2x2fs;
		std::unordered_map<std::string, Mat4x4d> Mat4x4ds;
		std::unordered_map<std::string, Mat3x3d> Mat3x3ds;
		std::unordered_map<std::string, Mat2x2d> Mat2x2ds;
		std::unordered_map<std::string, WPtr<Texture>> Textures;
	};

	class TWISTED_API Material :public TObject
	{
	public:
		Material() = default;
		Material(const MaterialData& materialData) :
			TObject(),
			m_data(materialData)
		{
		}

		Shader* GetShader() { return m_shader.get(); }

		void Clear()
		{
			m_data = MaterialData{};
			m_shader = nullptr;
		}
		void SetData(const MaterialData& data)
		{
			m_data = data;
		}
		void SetShader(Shader* shader)
		{
			m_shader = shader;
		}

		template<typename T>
		T Get(const std::string& key)
		{
			auto& map = getMap<T>();
			auto it = map.find(key);
			if (it != map.end())
				return it->second;
			return T{};
		}

		template<typename T>
		void Set(const std::string& key, T obj)
		{
			getMap<T>()[key] = obj;
		}
		void SetTexture(const std::string& key, Texture* tex)
		{
			getMap<WPtr<Texture>>()[key] = tex;
		}

		void ApplyUniforms()
		{
			if (!m_shader)
				return;

			for (const auto& uniform : m_shader->GetUniforms())
			{
				switch (uniform.Type)
				{
				case ShaderVarType::BOOL:
					m_shader->SetVar(uniform.UniformID, Get<bool>(uniform.Name));
					break;
				case ShaderVarType::UNSIGNED_INT:
					m_shader->SetVar(uniform.UniformID, Get<unsigned int>(uniform.Name));
					break;
				case ShaderVarType::INT:
					m_shader->SetVar(uniform.UniformID, Get<int>(uniform.Name));
					break;
				case ShaderVarType::FLOAT:
					m_shader->SetVar(uniform.UniformID, Get<float>(uniform.Name));
					break;
				case ShaderVarType::VEC2_F:
					m_shader->SetVar(uniform.UniformID, Get<Vec2f>(uniform.Name));
					break;
				case ShaderVarType::VEC3_F:
					m_shader->SetVar(uniform.UniformID, Get<Vec3f>(uniform.Name));
					break;
				case ShaderVarType::VEC4_F:
					m_shader->SetVar(uniform.UniformID, Get<Vec4f>(uniform.Name));
					break;
				case ShaderVarType::VEC2_D:
					m_shader->SetVar(uniform.UniformID, Get<Vec2d>(uniform.Name));
					break;
				case ShaderVarType::VEC3_D:
					m_shader->SetVar(uniform.UniformID, Get<Vec3d>(uniform.Name));
					break;
				case ShaderVarType::VEC4_D:
					m_shader->SetVar(uniform.UniformID, Get<Vec4d>(uniform.Name));
					break;
				case ShaderVarType::VEC2_I:
					m_shader->SetVar(uniform.UniformID, Get<Vec2i>(uniform.Name));
					break;
				case ShaderVarType::VEC3_I:
					m_shader->SetVar(uniform.UniformID, Get<Vec3i>(uniform.Name));
					break;
				case ShaderVarType::VEC4_I:
					m_shader->SetVar(uniform.UniformID, Get<Vec4i>(uniform.Name));
					break;
				case ShaderVarType::MAT4x4_F:
					m_shader->SetVar(uniform.UniformID, Get<Mat4x4f>(uniform.Name));
					break;
				case ShaderVarType::MAT3x3_F:
					m_shader->SetVar(uniform.UniformID, Get<Mat3x3f>(uniform.Name));
					break;
				case ShaderVarType::MAT2x2_F:
					m_shader->SetVar(uniform.UniformID, Get<Mat2x2f>(uniform.Name));
					break;
				case ShaderVarType::MAT4x4_D:
					m_shader->SetVar(uniform.UniformID, Get<Mat4x4d>(uniform.Name));
					break;
				case ShaderVarType::MAT3x3_D:
					m_shader->SetVar(uniform.UniformID, Get<Mat3x3d>(uniform.Name));
					break;
				case ShaderVarType::MAT2x2_D:
					m_shader->SetVar(uniform.UniformID, Get<Mat2x2d>(uniform.Name));
					break;
				case ShaderVarType::SAMPLER2D:
				{
					auto tex = Get<WPtr<Texture>>(uniform.Name);
					unsigned int texID = tex ? tex->GetTexID() : 0;
					m_shader->SetTex(uniform.UniformID, texID, uniform.TextureUnit);
					break;
				}
				default:
					TWISTED_WARN("Unsupported shader uniform type");
					break;
				}
			}
		}

		YAML::Node Serialize()const;

		void Deserialize(const YAML::Node& node);

	private:

		template<typename T>
		std::unordered_map<std::string, T>& getMap();
		template<typename T>
		const std::unordered_map<std::string, T>& getMap() const;

		template<> std::unordered_map<std::string, unsigned int>& getMap<unsigned int>() { return m_data.UnsignedInts; }
		template<> const std::unordered_map<std::string, unsigned int>& getMap<unsigned int>()const { return m_data.UnsignedInts; }

		template<> std::unordered_map<std::string, bool>& getMap<bool>() { return m_data.Bools; }
		template<> const std::unordered_map<std::string, bool>& getMap<bool>()const { return m_data.Bools; }

		template<> std::unordered_map<std::string, double>& getMap<double>() { return m_data.Doubles; }
		template<> const std::unordered_map<std::string, double>& getMap<double>()const { return m_data.Doubles; }

		template<> std::unordered_map<std::string, int>& getMap<int>() { return m_data.Ints; }
		template<> const std::unordered_map<std::string, int>& getMap<int>()const { return m_data.Ints; }

		template<> std::unordered_map<std::string, float>& getMap<float>() { return m_data.Floats; }
		template<> const std::unordered_map<std::string, float>& getMap<float>()const { return m_data.Floats; }

		template<> std::unordered_map<std::string, Vec2f>& getMap<Vec2f>() { return m_data.Vec2fs; }
		template<> const std::unordered_map<std::string, Vec2f>& getMap<Vec2f>()const { return m_data.Vec2fs; }

		template<> std::unordered_map<std::string, Vec3f>& getMap<Vec3f>() { return m_data.Vec3fs; }
		template<> const std::unordered_map<std::string, Vec3f>& getMap<Vec3f>() const { return m_data.Vec3fs; }

		template<> std::unordered_map<std::string, Vec4f>& getMap<Vec4f>() { return m_data.Vec4fs; }
		template<> const std::unordered_map<std::string, Vec4f>& getMap<Vec4f>() const { return m_data.Vec4fs; }

		template<> std::unordered_map<std::string, Vec2d>& getMap<Vec2d>() { return m_data.Vec2ds; }
		template<> const std::unordered_map<std::string, Vec2d>& getMap<Vec2d>()const { return m_data.Vec2ds; }

		template<> std::unordered_map<std::string, Vec3d>& getMap<Vec3d>() { return m_data.Vec3ds; }
		template<> const std::unordered_map<std::string, Vec3d>& getMap<Vec3d>() const { return m_data.Vec3ds; }

		template<> std::unordered_map<std::string, Vec4d>& getMap<Vec4d>() { return m_data.Vec4ds; }
		template<> const std::unordered_map<std::string, Vec4d>& getMap<Vec4d>() const { return m_data.Vec4ds; }

		template<> std::unordered_map<std::string, Vec2i>& getMap<Vec2i>() { return m_data.Vec2is; }
		template<> const std::unordered_map<std::string, Vec2i>& getMap<Vec2i>()const { return m_data.Vec2is; }

		template<> std::unordered_map<std::string, Vec3i>& getMap<Vec3i>() { return m_data.Vec3is; }
		template<> const std::unordered_map<std::string, Vec3i>& getMap<Vec3i>() const { return m_data.Vec3is; }

		template<> std::unordered_map<std::string, Vec4i>& getMap<Vec4i>() { return m_data.Vec4is; }
		template<> const std::unordered_map<std::string, Vec4i>& getMap<Vec4i>() const { return m_data.Vec4is; }

		template<> std::unordered_map<std::string, Mat2x2f>& getMap<Mat2x2f>() { return m_data.Mat2x2fs; }
		template<> const std::unordered_map<std::string, Mat2x2f>& getMap<Mat2x2f>()const { return m_data.Mat2x2fs; }

		template<> std::unordered_map<std::string, Mat3x3f>& getMap<Mat3x3f>() { return m_data.Mat3x3fs; }
		template<> const std::unordered_map<std::string, Mat3x3f>& getMap<Mat3x3f>()const { return m_data.Mat3x3fs; }

		template<> std::unordered_map<std::string, Mat4x4f>& getMap<Mat4x4f>() { return m_data.Mat4x4fs; }
		template<> const std::unordered_map<std::string, Mat4x4f>& getMap<Mat4x4f>()const { return m_data.Mat4x4fs; }

		template<> std::unordered_map<std::string, Mat2x2d>& getMap<Mat2x2d>() { return m_data.Mat2x2ds; }
		template<> const std::unordered_map<std::string, Mat2x2d>& getMap<Mat2x2d>()const { return m_data.Mat2x2ds; }

		template<> std::unordered_map<std::string, Mat3x3d>& getMap<Mat3x3d>() { return m_data.Mat3x3ds; }
		template<> const std::unordered_map<std::string, Mat3x3d>& getMap<Mat3x3d>()const { return m_data.Mat3x3ds; }

		template<> std::unordered_map<std::string, Mat4x4d>& getMap<Mat4x4d>() { return m_data.Mat4x4ds; }
		template<> const std::unordered_map<std::string, Mat4x4d>& getMap<Mat4x4d>()const { return m_data.Mat4x4ds; }

		template<>std::unordered_map<std::string, WPtr<Texture>>& getMap<WPtr<Texture>>() { return m_data.Textures; }
		template<>const std::unordered_map<std::string, WPtr<Texture>>& getMap<WPtr<Texture>>() const { return m_data.Textures; }

		MaterialData m_data;
		WPtr<Shader> m_shader = nullptr;


	};
}
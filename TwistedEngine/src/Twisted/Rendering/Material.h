#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/BaseObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Data/Color.h"

#include <unordered_map>
#include <string>

namespace Twisted
{
	struct MaterialData
	{
		std::unordered_map<std::string, bool> Bools;
		std::unordered_map<std::string, int> Ints;
		std::unordered_map<std::string, float> Floats;
		std::unordered_map<std::string, Vec4f> Vec4s;
		std::unordered_map<std::string, Vec3f> Vec3s;
		std::unordered_map<std::string, Vec2f> Vec2s;
		std::unordered_map<std::string, Mat4x4f> Mats;
		std::unordered_map<std::string, ObjectID> Textures;
	};

	class TWISTED_API Material :public BaseObject
	{
	public:
		Material(ObjectID id);
		Material(ObjectID id, const MaterialData& materialData);

		const ObjectID GetShader()const { return m_shader; }

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

		void SetData(const MaterialData& data)
		{
			m_data = data;
		}

	private:

		template<typename T>
		std::unordered_map<std::string, T>& getMap();

		template<typename T>
		const std::unordered_map<std::string, T>& getMap() const;

		template<> std::unordered_map<std::string, bool>& getMap<bool>() { return m_data.Bools; }
		template<> const std::unordered_map<std::string, bool>& getMap<bool>()const { return m_data.Bools; }

		template<> std::unordered_map<std::string, int>& getMap<int>() { return m_data.Ints; }
		template<> const std::unordered_map<std::string, int>& getMap<int>()const { return m_data.Ints; }

		template<> std::unordered_map<std::string, float>& getMap<float>() { return m_data.Floats; }
		template<> const std::unordered_map<std::string, float>& getMap<float>()const { return m_data.Floats; }

		template<> std::unordered_map<std::string, Vec2f>& getMap<Vec2f>() { return m_data.Vec2s; }
		template<> const std::unordered_map<std::string, Vec2f>& getMap<Vec2f>()const { return m_data.Vec2s; }

		template<> std::unordered_map<std::string, Vec3f>& getMap<Vec3f>() { return m_data.Vec3s; }
		template<> const std::unordered_map<std::string, Vec3f>& getMap<Vec3f>() const { return m_data.Vec3s; }

		template<> std::unordered_map<std::string, Vec4f>& getMap<Vec4f>() { return m_data.Vec4s; }
		template<> const std::unordered_map<std::string, Vec4f>& getMap<Vec4f>() const { return m_data.Vec4s; }

		template<> std::unordered_map<std::string, Mat4x4f>& getMap<Mat4x4f>() { return m_data.Mats; }
		template<> const std::unordered_map<std::string, Mat4x4f>& getMap<Mat4x4f>()const { return m_data.Mats; }

		MaterialData m_data;
		ObjectID m_shader;
	};
}

//m_bools[uniVar.Name] = ShaderValue<bool>{ static_cast<int>(uniVar.UniformID) };

		//void SetShader(ObjectID shader) { m_shader = shader; }
		//void SetBool(const std::string& varName, bool value) { m_data.Bools[varName] = value; }
		//void SetInt(const std::string& varName, int value) { m_data.Ints[varName] = value; }
		//void SetFloat(const std::string& varName, float value) { m_data.Floats[varName] = value; }
		//void SetVec2f(const std::string& varName, Vec2f value) { m_data.Vec2s[varName] = value; }
		//void SetVec3f(const std::string& varName, Vec3f value) { m_data.Vec3s[varName] = value; }
		//void SetVec4f(const std::string& varName, Vec4f value) { m_data.Vec4s[varName] = value; }
		//void SetMat4x4f(const std::string& varName, Mat4x4f value) { m_data.Mats[varName] = value; }
		//void SetTexture(const std::string& texName, ObjectID texture) { m_data.Textures[texName] = texture; }

		//bool GetBool(const std::string& varName, bool defaultValue)const
		//{
		//	auto it = m_data.Bools.find(varName);
		//	if (it != m_data.Bools.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//int GetInt(const std::string& varName, int defaultValue)const
		//{
		//	auto it = m_data.Ints.find(varName);
		//	if (it != m_data.Ints.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//float GetFloat(const std::string& varName, float defaultValue)const
		//{
		//	auto it = m_data.Floats.find(varName);
		//	if (it != m_data.Floats.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//Vec2f GetVec2f(const std::string& varName, Vec2f defaultValue) const
		//{
		//	auto it = m_data.Vec2s.find(varName);
		//	if (it != m_data.Vec2s.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//Vec3f GetVec3f(const std::string& varName, Vec3f defaultValue) const
		//{
		//	auto it = m_data.Vec3s.find(varName);
		//	if (it != m_data.Vec3s.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//Vec4f GetVec4f(const std::string& varName, Vec4f defaultValue) const
		//{
		//	auto it = m_data.Vec4s.find(varName);
		//	if (it != m_data.Vec4s.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//Mat4x4f GetMat4x4f(const std::string& varName, Mat4x4f defaultValue) const
		//{
		//	auto it = m_data.Mats.find(varName);
		//	if (it != m_data.Mats.end())
		//		return it->second;
		//	return defaultValue;
		//}
		//const ObjectID GetTexture(const std::string& texName, ObjectID defaultValue) const
		//{
		//	auto it = m_data.Textures.find(texName);
		//	if (it != m_data.Textures.end())
		//		return it->second;
		//	return defaultValue;
		//}

		//template<typename T>
		//T getDefault() { retur T{}; }

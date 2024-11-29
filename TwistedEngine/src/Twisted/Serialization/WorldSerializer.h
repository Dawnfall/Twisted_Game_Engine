#pragma once
#include "AppCore.h"
#include "twistedpch.h"
#include "SerializationBuffer.h"

namespace Twisted
{
	class World;
	class WorldSerializer
	{
	public:

		void SerializeWorld(const World* world);
		SRef<World> DeserializeWorld(SerializationBuffer& serializer);

		size_t RegisterObject(void* obj)
		{
			auto it = m_objectToIds.find(obj);
			if (it != m_objectToIds.end())
				return it->second;

			size_t id = currId++;
			m_idsToObjects[id] = obj;
			m_objectToIds[obj] = id;
			return id;
		}

		template<typename T>
		T* GetObject(size_t id)
		{
			auto it = m_idsToObjects.find(id);
			if (it != m_idsToObjects.end())
				return static_cast<T*>(it->second);
			return nullptr;
		}

		size_t currId = 0;
		std::unordered_map<size_t, void*> m_idsToObjects;
		std::unordered_map<void*, size_t> m_objectToIds;
	};

	template<typename T>
	void Serialize(const T& obj, SerializationBuffer& serializer) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }

	template<typename T>
	void Deserialize(T& obj, SerializationBuffer& serializer) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }

	template<typename T>
	void PreSerialize(const T& obj, SerializationBuffer& serializer) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }

	template<typename T>
	void PostDeserialize(T& obj, SerializationBuffer& serializer) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }

}
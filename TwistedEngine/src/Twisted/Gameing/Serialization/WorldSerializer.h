#pragma once
#include "AppCore.h"
#include "Twisted/Data/SerializationBuffer.h"
#include "Twisted/Gameing/World.h"

//class WorldSerializer
//{
//public:
//	size_t RegisterObject(void* obj)
//	{
//		auto it = m_objectToIds.find(obj);
//		if (it != m_objectToIds.end())
//			return it->second;

//		size_t id = currId++;
//		m_idsToObjects[id] = obj;
//		m_objectToIds[obj] = id;
//		return id;
//	}

//	template<typename T>
//	T* GetObject(size_t id)
//	{
//		auto it = m_idsToObjects.find(id);
//		if (it != m_idsToObjects.end())
//			return static_cast<T*>(it->second);
//		return nullptr;
//	}

//	size_t currId = 0;
//	std::unordered_map<size_t, void*> m_idsToObjects;
//	std::unordered_map<void*, size_t> m_objectToIds;
//};
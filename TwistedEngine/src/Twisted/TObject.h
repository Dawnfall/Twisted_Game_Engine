#pragma once

#include "AppCore.h"
#include "ObjectID.h"
#include <memory>
#include <vector>

namespace Twisted
{
	class TWISTED_API TObject
	{
	public:
		TObject(const std::string& name);
		virtual ~TObject();

		// No copy
		TObject(const TObject&) = delete;
		TObject& operator=(const TObject&) = delete;

		// Allow move
		TObject(TObject&&) noexcept = delete;
		TObject& operator=(TObject&&) noexcept = delete;

		ObjectID GetID() const { return m_id; }

		template<typename T>
		T* dynamic_as() { return dynamic_cast<T*>(this); }

		template<typename T>
		T* static_as() { return static_cast<T*>(this); }

		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName()const { return m_name; }

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
	private:
		ObjectID m_id;
		std::string m_name = "";

		friend class ObjectManager;
	};
}


#include "TObject.h"
#include "ObjectManager.h"

namespace Twisted
{
	TObject::TObject(const std::string& name) :
		m_name(name) 
	{}

	TObject::~TObject()
	{
		ObjectManager::DestroyID(GetID());
	}
}
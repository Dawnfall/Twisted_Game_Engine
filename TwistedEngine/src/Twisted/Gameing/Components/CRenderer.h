#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/RegisterLayer/ObjectManager.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

namespace Twisted
{
	class RegistrationLayer;
}

namespace Twisted
{
	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(EntityID entity, World* world) :AComponent(entity,world) {}
		
		ObjectID& GetSharedMesh() { return m_mesh; }
		void SetSharedMesh(ObjectID mesh) { m_mesh = mesh; }
		ObjectID& GetSharedMaterial() { return m_material; }
		void SetSharedMaterial(ObjectID material) { m_material = material; }

		//Serialization
		void Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const override;
		void Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) override;

	private:

		ObjectID m_material;
		ObjectID m_mesh;
	};

	//template<>
	//inline void Serialize(const CRenderer& renderer,BinSerializer& serializer)
	//{
	//	
	//}

	//template<>
	//inline void Deserialize(CRenderer& renderer,BinSerializer& serializer)
	//{

	//}
}
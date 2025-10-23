#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"

namespace Twisted
{
	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(Entity entity) :AComponent(entity) {}

		Mesh* GetSharedMesh() { return m_mesh; }
		void SetSharedMesh(Mesh* mesh) { m_mesh = mesh; }
		Material* GetSharedMaterial() { return m_material; }
		void SetSharedMaterial(Material* material) { m_material = material; }

		//Serialization
		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer) override;

	private:

		Material* m_material = nullptr;
		Mesh* m_mesh = nullptr;
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
#include "pch.h"
#include "CTransform.h"
#include "Twisted/GameCore.h"
#include <EnTT/entt.hpp>

namespace Twisted
{
	CTransform::CTransform(entt::entity entity) :
		AComponent(entity),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(1, 0, 0, 0),
		m_scale(1.0f, 1.0f, 1.0f),
		m_parentID(entt::null)
		//m_children()
	{}
	CTransform::CTransform(entt::entity entity, const Vec3f& position, const Quat& rotation, const Vec3f& scale) :
		AComponent(entity),
		m_position(position),
		m_rotation(rotation),
		m_scale(scale),
		m_parentID(entt::null)
		//m_children()
	{}

	//Transform::Transform(Transform* parent)
	//{
	//	parent->AddChild(this);
	//}




}
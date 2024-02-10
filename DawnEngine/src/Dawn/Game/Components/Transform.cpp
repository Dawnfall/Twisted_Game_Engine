#include "Transform.h"

namespace Dawn
{
	Transform::Transform()
	{
		m_position = glm::vec3();
		m_rotation = glm::quat(1, 0, 0, 0);
		m_scale = glm::vec3(1, 1, 1);
	}
	Transform::Transform(const glm::vec3 position, const glm::quat rotation, const glm::vec3 scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
	}
	//Transform::Transform(Transform* parent)
	//{
	//	parent->AddChild(this);
	//}




}
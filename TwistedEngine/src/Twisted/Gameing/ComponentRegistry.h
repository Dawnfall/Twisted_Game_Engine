#pragma once
#include <tuple>

#include "Components/CTransform.h"
#include "Components/CName.h"
#include "Components/CCamera.h"
#include "Components/CRenderer.h"
#include "Components/CLight.h"

namespace Twisted
{
	template <typename... Ts>
	struct type_list
	{
	};

	using AllComponents = type_list<
		CTransform,
		CName,
		CCamera,
		CRenderer,
		CLight
	>;


	template <typename F, typename... Ts>
	void for_each_type(type_list<Ts...>, F&& f) {
		(f.template operator() < Ts > (), ...);
	}
}
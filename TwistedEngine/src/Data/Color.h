#pragma once

namespace Twisted
{
	struct Color
	{
		float r, g, b, a;

		bool operator== (const Color& other)const
		{
			return
				r == other.r &&
				g == other.g &&
				b == other.b &&
				a == other.a;
		}

		bool operator!= (const Color& other)const { return !((*this) == other); }
	};

	const Color black = { 0.0f,0.0f,0.0f,1.0f };
	const Color white = { 1.0f,1.0f,1.0f,1.0f };
	const Color blue = { 0.0f,0.0f,1.0f,1.0f };
	const Color red = { 1.0f,0.0f,0.0f,1.0f };
	const Color green = { 0.0f,1.0f,0.0f,1.0f };
}


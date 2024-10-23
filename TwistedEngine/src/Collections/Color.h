#pragma once

namespace Twisted::Colors
{
	struct Color
	{
		float r, g, b, a;
	};

	const Color black = { 0.0f,0.0f,0.0f,1.0f };
	const Color white = { 1.0f,1.0f,1.0f,1.0f };
	const Color blue = { 0.0f,0.0f,1.0f,1.0f };
	const Color red = { 1.0f,0.0f,0.0f,1.0f };
	const Color green = { 0.0f,1.0f,0.0f,1.0f };
}
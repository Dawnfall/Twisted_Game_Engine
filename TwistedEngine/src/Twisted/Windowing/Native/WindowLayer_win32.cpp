#include "AppCore.h"
#ifndef GLFW_INCLUDE_NONE

#include "Twisted/Windowing/WindowLayer.h"

namespace Twisted
{
	WindowLayer::WindowLayer(Application* app) :Layer(app){}
	WindowLayer::~WindowLayer() {}
}

#endif


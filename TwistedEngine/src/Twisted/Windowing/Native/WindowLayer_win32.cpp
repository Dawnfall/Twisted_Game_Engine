#include "AppCore.h"
#ifdef NATIVE_USE

#include "Twisted/Windowing/WindowLayer.h"

namespace Twisted
{
	WindowLayer::WindowLayer(Application* app) :Layer(app){}
	WindowLayer::~WindowLayer() {}
}

#endif
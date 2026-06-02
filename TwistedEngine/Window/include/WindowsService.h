#pragma once
#include "AppCore.h"
#include "WindowEvents.h"
#include "Utils/Event.h"

namespace Twisted
{
	class TWISTED_API WindowsService
	{
	public:
		virtual ~WindowsService() = default;

		virtual void DispatchEvent(const KeyEvent& e) = 0;
		virtual void DispatchEvent(const MouseButtonEvent& e) = 0;
		virtual void DispatchEvent(const MouseMoveEvent& e) = 0;
		virtual void DispatchEvent(const MouseWheelEvent& e) = 0;
		virtual void DispatchEvent(const WindowCloseEvent& e) = 0;
		virtual void DispatchEvent(const WindowResizeEvent& e) = 0;
		virtual void DispatchEvent(const WindowFocusEvent& e) = 0;
		virtual void DispatchEvent(const FilesDroppedEvent& e) = 0;
		virtual Event<void*>& GetPollMsgEvent() = 0;
	};
}

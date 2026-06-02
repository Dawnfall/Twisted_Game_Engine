#pragma once
#include "AppCore.h"
#include "Application/Service.h"
#include "Data/RenderContext.h"
#include "WindowsService.h"
#include "Window.h"
#include "Utils/Event.h"
#include <queue>
#include <string>

namespace Twisted
{
	class TWISTED_API RenderService : public Service, public WindowsService
	{
	public:
		RenderService(Application* app, int priority);

		void OnInit() override;
		void OnTerminate() override;

		void PollEvents();
		Window* GetWindow() { return m_window ? m_window.get() : nullptr; }
		Window* CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position);
		void DestroyWindow(Window* window);

		Event<Window*> WindowCreatedEvent;

		void DispatchEvent(const KeyEvent& e) override;
		void DispatchEvent(const MouseButtonEvent& e) override;
		void DispatchEvent(const MouseMoveEvent& e) override;
		void DispatchEvent(const MouseWheelEvent& e) override;
		void DispatchEvent(const WindowCloseEvent& e) override;
		void DispatchEvent(const WindowResizeEvent& e) override;
		void DispatchEvent(const WindowFocusEvent& e) override;
		void DispatchEvent(const FilesDroppedEvent& e) override;
		Event<void*>& GetPollMsgEvent() override { return m_pollMsgEvent; }

		void SubmitContext(RenderContext context);
		void Render();
		void WaitIdle() const;

	private:
		URef<Window> m_window;
		std::queue<RenderContext> m_queue;
		Event<void*> m_pollMsgEvent;
	};
}

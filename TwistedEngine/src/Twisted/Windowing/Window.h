#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Input.h"
#include "GraphicsContext.h"

namespace Twisted
{
	class TWISTED_API Window
	{
	public:
		Window(const std::string& title, Vec2i size, Vec2i position);
		Window(const Window& other) = delete;
		Window(Window&& other) = default;
		~Window();

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = default;

		Vec2i GetSize()const;
		Vec2i GetPosition()const;
		std::string GetTitle()const;
		GraphicsContext* GetContext() { return m_context.get(); }

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);

		void Maximize();
		void SetFullScreen(bool isFullScreen); //TODO:... for now only on mainMonitor
		void SetWindowed(Vec2i size, Vec2i pos);

		void* GetRawPointer();
		void* GetNativeHandle();

		bool CloseWindow();

		Event<> CloseWindowEvent;
		Event<Vec2i> WindowResizeEvent;

		inline static Event<void*> PollMsgEvent;
		static void PollEvents();

	private:
		void* m_pointer = nullptr;
		URef<GraphicsContext> m_context = nullptr;
	};
}

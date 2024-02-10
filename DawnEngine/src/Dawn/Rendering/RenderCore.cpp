#include "RenderCore.h"

namespace Dawn
{
	void RenderCore::Init()
	{
		bool isWindowInit = Window::Init();
		//DAWN_DEBUG_ASSERT(isWindowInit);
		m_window = std::unique_ptr<Window>(Window::Create());

		//TODO: prolly move from here
		m_window->closeWindowEvent.AddListener([this]() {m_window->Close(); });

		DAWN_INFO("RenderCore Init success!");
	}

	void RenderCore::Update()
	{
		m_window->Clear();



		m_window->OnUpdate();
	}



}
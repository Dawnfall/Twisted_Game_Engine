#pragma once
#include "Twisted/Application/Application.h"

#include "Twisted/Rendering/RenderLayer.h"
#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

namespace Twisted::Standalone
{
	class StandaloneApplication : public Application
	{
	private:

		void OnCreate() override
		{
			RegisterLayers();
		}

		void OnBeforeRun() override
		{

		}

		void OnFrame() override
		{

		}

		void OnTerminate()override
		{

		}

	private:

		void RegisterLayers()
		{
			m_windowLayer = AddLayer<WindowLayer>();
			m_renderLayer = AddLayer<RenderLayer>();
			m_assetsLayer = AddLayer<AssetsLayer>();
		}

	private:


		Window* m_window = nullptr;

		WindowLayer* m_windowLayer = nullptr;
		RenderLayer* m_renderLayer = nullptr;
		AssetsLayer* m_assetsLayer = nullptr;

	};

}

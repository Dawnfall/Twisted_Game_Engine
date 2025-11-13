#pragma once

#include "AppCore.h"

#include "Twisted/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"

namespace Twisted
{


	class TWISTED_API FrameBuffer :public TObject
	{
	public:
		FrameBuffer(const std::string& name, Vec2i size) :
			TObject(name),
			m_size(size)
		{
		}

		bool IsValid()const
		{
			return m_id != 0 && m_tex && m_tex->IsValid();
		}

		const Texture* GetTexture()const { return m_tex.get(); }
		Vec2i GetSize()const { return m_size; }

		void SetSize(const Vec2i& size);

		void Bind();
		void UnBind()const;

		void OnCreate() override;
		void OnDestroy() override
		{
			Destroy();
		}

		void Blit(unsigned int destID) const;

	private:
		void Update();
		void Destroy();

		Vec2i m_size;

		WPtr<Texture> m_tex;
		unsigned int m_id = 0;
		unsigned int m_rbo = 0;


		bool m_isDirty = true;
	};
}


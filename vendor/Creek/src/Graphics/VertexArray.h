#pragma once
#include "Walnut/crkpch.h"
#include "Walnut/Graphics/VertexBuffer.h"
#include "Walnut/Graphics/VertexBufferLayout.h"

namespace Creek
{
	namespace Graphics
	{
		using VertexBuffer = Creek::Graphics::VertexBuffer;
		using VertexBufferLayout = Creek::Graphics::VertexBufferLayout;

		class VertexArray
		{
		public:
			VertexArray();
			~VertexArray();

			void Bind() const;
			void Unbind() const;
			void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		private:
			unsigned int m_RendererID;
		};
	}
}


#pragma once
#include "Walnut/crkpch.h"
namespace Creek
{
	namespace Graphics
	{

		class VertexBuffer
		{
		public:
			VertexBuffer(const void* data, unsigned int size);
			~VertexBuffer();

			void Bind()const;
			void Unbind()const;
		private:
			unsigned int vbo;
		};
	}
}


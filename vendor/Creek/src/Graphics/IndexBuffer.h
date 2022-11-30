#pragma once
#include "Walnut/crkpch.h"
namespace Creek
{
	namespace Graphics
	{
		class IndexBuffer
		{
		public:
			IndexBuffer(const unsigned int* size, unsigned int count);
			~IndexBuffer();

			void Bind()const;
			void Unbind()const;

			inline unsigned int GetCount() const { return count; }
		private:
			unsigned int ibo;
			unsigned int count;
		};
	}
}


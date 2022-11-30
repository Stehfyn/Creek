#pragma once
#include "Walnut/Graphics/VertexArray.h"
#include "Walnut/Graphics/IndexBuffer.h"
#include "Walnut/Graphics/Shader.h"

namespace Creek
{	
	namespace Graphics
	{
		class Renderer
		{
		public:
			void Clear() const;
			void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		};
	}
}



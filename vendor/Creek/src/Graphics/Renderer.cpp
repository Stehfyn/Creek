#include "Renderer.h"
namespace Creek
{
	namespace Graphics
	{
		void Renderer::Clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
		{
			shader.Bind();

			va.Bind();
			ib.Bind();

			glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

			//gl unbinding isn't strictly necessary by the the order in which we bind every frame
			//
			//ib.Unbind();
			//va.Unbind();
			//shader.Unbind();
		}
	}
}

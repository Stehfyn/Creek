#include "VertexArray.h"

Creek::Graphics::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

Creek::Graphics::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void Creek::Graphics::VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void Creek::Graphics::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Creek::Graphics::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	(*this).Bind();
	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

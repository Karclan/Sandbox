#pragma once

#include "../../VertexArrayObject.h"
#include "../../DataBuffer.h"

namespace Graphics
{
	class OpenGLVertexArrayObject : public VertexArrayObject
	{
	public:
		OpenGLVertexArrayObject();
		virtual ~OpenGLVertexArrayObject();

		void Bind() override;
		void UnBind() override;


		void AddVertexBuffer( std::shared_ptr<VertexBuffer> const & vertexBuffer ) override;
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> const & indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>> & GetVertexBuffers() const override { return m_VertexBuffers; }
		const std::shared_ptr<IndexBuffer> & GetIndexBuffer() const override { return m_IndexBuffer; }

		bool HasIndicies() const override { return m_IndexBuffer != 0; }

	private:
		uint32_t m_VertexBufferIndex { 0 };
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers {};
		std::shared_ptr<IndexBuffer> m_IndexBuffer {};
	};
}
#pragma once

#include "../../DataBuffer.h"

namespace Graphics
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer( uint32_t size );
		OpenGLVertexBuffer( float * vertices, uint32_t size );
		OpenGLVertexBuffer( float const * vertices, uint32_t size );
		virtual ~OpenGLVertexBuffer();

		void SetData( const void * data, uint32_t dataSize ) override;

		BufferLayout const & GetLayout() const override { return m_Layout; }
		void SetLayout( BufferLayout const & layout ) override;

		uint32_t GetVertexCount() const override { return m_vertexCount; }

		void Bind() override;
		void UnBind() override;

	private:
		void CreateBindAndSetBuffer( uint32_t & id, uint32_t size, void const * vertices, auto drawType );

		BufferLayout m_Layout {};
		uint32_t m_vertexCount { 0u };
		uint32_t m_dataSize { 0u };
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer( uint32_t * indicies, uint32_t cout );
		OpenGLIndexBuffer( uint32_t const * indicies, uint32_t cout );
		virtual ~OpenGLIndexBuffer();

		void Bind() override;
		void UnBind() override;

		uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_Count { 0u };

	};
}

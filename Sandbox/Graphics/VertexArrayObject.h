#pragma once

#include "DataBuffer.h"

namespace Graphics
{
	class VertexArrayObject : public DataBuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void AddVertexBuffer( std::shared_ptr<VertexBuffer> const & vertexBuffer ) = 0;
		virtual void SetIndexBuffer( std::shared_ptr<IndexBuffer> const & indexBuffer ) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>> & GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer> & GetIndexBuffer() const = 0;

		virtual bool HasIndicies() const = 0;

		static std::shared_ptr<VertexArrayObject> Create();
	};
}

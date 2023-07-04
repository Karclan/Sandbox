#include "DataBuffer.h"

#include "RendererAPI.h"
#include "../Utils/Assert.h"

#include "Platform/OpenGL/OpenGLDataBuffer.h"

namespace Graphics
{

	std::shared_ptr<Graphics::VertexBuffer> VertexBuffer::Create( uint32_t size )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLVertexBuffer>( size );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}

	std::shared_ptr<Graphics::VertexBuffer> VertexBuffer::Create( float * verticies, uint32_t size )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLVertexBuffer>( verticies, size );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}

	std::shared_ptr<Graphics::VertexBuffer> VertexBuffer::Create( float const * verticies, uint32_t size )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLVertexBuffer>( verticies, size );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}

	std::shared_ptr<Graphics::IndexBuffer> IndexBuffer::Create( uint32_t * indicies, uint32_t count )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLIndexBuffer>( indicies, count );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}

	std::shared_ptr<Graphics::IndexBuffer> IndexBuffer::Create( uint32_t const * indicies, uint32_t count )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLIndexBuffer>( indicies, count );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}

}

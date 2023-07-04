#include "VertexArrayObject.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArrayObject.h"

#include "../Utils/Assert.h"

namespace Graphics
{
	std::shared_ptr<Graphics::VertexArrayObject> VertexArrayObject::Create()
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<Graphics::OpenGLVertexArrayObject>();
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}
}

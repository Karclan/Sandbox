#include "UniformBufferObject.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLUniformBufferObject.h"

#include "../Utils/Assert.h"

namespace Graphics
{
	std::shared_ptr<Graphics::UniformBufferObject> UniformBufferObject::Create( uint32_t size, uint32_t binding )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLUniformBufferObject>( size, binding );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
				SB_CORE_ASSERT( false, "Unhandled Api" );
				return nullptr;
		}
	}
}

#include "Framebuffer.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include "../Utils/Assert.h"

namespace Graphics
{

	FramebufferTextureSpecification::FramebufferTextureSpecification( FramebufferTextureFormat format ) 
		: TextureFormat( format )
	{
	}

	FramebufferAttachmentSpecification::FramebufferAttachmentSpecification( std::initializer_list<FramebufferTextureSpecification> textureAttachments )
		: TextureAttachments( textureAttachments )
	{
	}

	std::shared_ptr<Graphics::Framebuffer> Framebuffer::Create( const FramebufferSpecification & spec )
	{
		switch ( RendererAPI::GetAPI() )
		{
			case Graphics::RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLFramebuffer>( spec );
			case Graphics::RendererAPI::API::DirectX:
			case Graphics::RendererAPI::API::Vulkan:
			case Graphics::RendererAPI::API::None:
			default:
			{
				SB_CORE_ASSERT( false, "Unhandled API" );
				return nullptr;
			}
		}
	}
}

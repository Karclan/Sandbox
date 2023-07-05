#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace Graphics
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		// Color
		RGBA8,
		RED_INTEGER,
		// Depth/stencil
		DEPTH24STENCIL8,
		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification( FramebufferTextureFormat format );
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification( std::initializer_list<FramebufferTextureSpecification> textureAttachments );

		std::vector<FramebufferTextureSpecification> TextureAttachments;
	};

	struct FramebufferSpecification
	{
		bool SwapChainTarget = false;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize( uint32_t width, uint32_t height ) = 0;
		virtual int ReadPixel( uint32_t attachmentIndex, int x, int y ) = 0;

		virtual void ClearAttachment( uint32_t attachmentIndex, int value ) = 0;

		virtual uint32_t GetColorAttachmentRendererID( uint32_t index = 0 ) const = 0;

		virtual FramebufferSpecification const & GetSpecification() const = 0;

		static std::shared_ptr<Framebuffer> Create( FramebufferSpecification const & spec );
	};

}

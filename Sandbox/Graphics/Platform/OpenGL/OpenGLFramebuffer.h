#pragma once

#include "../../Framebuffer.h"

#include <vector>

namespace Graphics
{
	class OpenGLFramebuffer : public Framebuffer
	{

	public:
		OpenGLFramebuffer( FramebufferSpecification const & spec );
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		void Resize( uint32_t width, uint32_t height ) override;
		int ReadPixel( uint32_t attachmentIndex, int x, int y ) override;

		void ClearAttachment( uint32_t attachmentIndex, int value ) override;
		uint32_t GetColorAttachmentRendererID( uint32_t index = 0 ) const override;
		FramebufferSpecification const & GetSpecification() const override;

	private:
		uint32_t m_Id { 0 };
		uint32_t m_DepthAttachment { 0 };

		FramebufferSpecification m_Specification {};
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		std::vector<uint32_t> m_ColourAttachments {};
		std::vector<FramebufferTextureSpecification> m_ColourAttachmentSpecifications {};
	};
}

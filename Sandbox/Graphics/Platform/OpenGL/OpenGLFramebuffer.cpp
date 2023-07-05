#include "OpenGLFramebuffer.h"

#include "../Utils/Assert.h"

#include "glad/glad.h"

namespace Graphics
{
	uint32_t constexpr s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		GLenum TextureTarget( bool multisampled )
		{
			if ( multisampled )
			{
				return GL_TEXTURE_2D_MULTISAMPLE;
			}
			else
			{
				return GL_TEXTURE_2D;
			}
		}

		void CreateTextures( bool multisampled, uint32_t * outID, uint32_t count )
		{
			glCreateTextures( TextureTarget( multisampled ), count, outID );
		}

		void BindTexture( bool multisampled, uint32_t id )
		{
			glBindTexture( TextureTarget( multisampled ), id );
		}

		void AttachColorTexture( uint32_t id, uint32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index )
		{
			bool const multisampled = samples > 1;
			if ( multisampled )
			{
				glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE );
			}
			else
			{
				glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr );

				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			}

			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget( multisampled ), id, 0 );
		}

		void AttachDepthTexture( uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height )
		{
			bool const multisampled = samples > 1;
			if ( multisampled )
			{
				glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE );
			}
			else
			{
				glTexStorage2D( GL_TEXTURE_2D, 1, format, width, height );

				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			}

			glFramebufferTexture2D( GL_FRAMEBUFFER, attachmentType, TextureTarget( multisampled ), id, 0 );
		}

		bool IsDepthFormat( FramebufferTextureFormat format )
		{
			switch ( format )
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					return true;
			}

			return false;
		}

		GLenum FBTextureFormatToGL( FramebufferTextureFormat format )
		{
			switch ( format )
			{
				case FramebufferTextureFormat::RGBA8:
					return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER:
					return GL_RED_INTEGER;
			}

			SB_CORE_ASSERT( false );
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer( FramebufferSpecification const & InSpec )
		: m_Specification( InSpec )
	{
		for ( auto const & spec : m_Specification.Attachments.TextureAttachments )
		{
			if ( !Utils::IsDepthFormat( spec.TextureFormat ) )
			{
				m_ColourAttachmentSpecifications.emplace_back( spec );
			}
			else
			{
				m_DepthAttachmentSpecification = spec;
			}
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers( 1, &m_Id );

		glDeleteTextures( static_cast< GLsizei >( m_ColourAttachments.size() ), m_ColourAttachments.data() );
		glDeleteTextures( 1, &m_DepthAttachment );
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if ( m_Id )
		{
			glDeleteFramebuffers( 1, &m_Id );
			glDeleteTextures( static_cast< GLsizei >( m_ColourAttachments.size() ), m_ColourAttachments.data() );
			glDeleteTextures( 1, &m_DepthAttachment );

			m_ColourAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers( 1, &m_Id );
		glBindFramebuffer( GL_FRAMEBUFFER, m_Id );

		bool const multisample = m_Specification.Samples > 1;

		// Attachments
		if ( m_ColourAttachmentSpecifications.size() )
		{
			m_ColourAttachments.resize( m_ColourAttachmentSpecifications.size() );
			Utils::CreateTextures( multisample, m_ColourAttachments.data(), static_cast< uint32_t >( m_ColourAttachments.size() ) );

			for ( size_t i = 0; i < m_ColourAttachments.size(); i++ )
			{
				Utils::BindTexture( multisample, m_ColourAttachments[ i ] );
				switch ( m_ColourAttachmentSpecifications[ i ].TextureFormat )
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture( m_ColourAttachments[ i ],
							m_Specification.Samples,
							GL_RGBA8,
							GL_RGBA,
							m_Specification.Width,
							m_Specification.Height,
							static_cast< int >( i ) );
						break;

					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture( m_ColourAttachments[ i ],
							m_Specification.Samples,
							GL_R32I,
							GL_RED_INTEGER,
							m_Specification.Width,
							m_Specification.Height,
							static_cast< int >( i ) );
						break;
				}
			}
		}

		if ( m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None )
		{
			Utils::CreateTextures( multisample, &m_DepthAttachment, 1 );
			Utils::BindTexture( multisample, m_DepthAttachment );


			switch ( m_DepthAttachmentSpecification.TextureFormat )
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture( m_DepthAttachment, m_Specification.Samples,
						GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
						m_Specification.Width, m_Specification.Height );
					break;
			}
		}

		if ( m_ColourAttachments.size() > 1 )
		{
			SB_CORE_ASSERT( m_ColourAttachments.size() <= 4 );
			GLenum buffers[ 4 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers( static_cast< GLsizei >( m_ColourAttachments.size() ), buffers );
		}
		else if ( m_ColourAttachments.empty() )
		{
			// Only depth-pass
			glDrawBuffer( GL_NONE );
		}

		SB_CORE_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!" );

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_Id );
		glViewport( 0, 0, m_Specification.Width, m_Specification.Height );
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void OpenGLFramebuffer::Resize( uint32_t width, uint32_t height )
	{
		if ( width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize )
		{
			SB_CORE_WARN( "Attempted to rezize framebuffer to {}, {}", width, height );
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel( uint32_t attachmentIndex, int x, int y )
	{
		SB_CORE_ASSERT( attachmentIndex < m_ColourAttachments.size() );

		glReadBuffer( GL_COLOR_ATTACHMENT0 + attachmentIndex );
		int pixelData;
		glReadPixels( x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData );
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment( uint32_t attachmentIndex, int value )
	{
		SB_CORE_ASSERT( attachmentIndex < m_ColourAttachments.size() );

		auto const & spec = m_ColourAttachmentSpecifications[ attachmentIndex ];
		glClearTexImage( m_ColourAttachments[ attachmentIndex ], 0,
			Utils::FBTextureFormatToGL( spec.TextureFormat ), GL_INT, &value );

	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID( uint32_t index /*= 0 */ ) const
	{
		SB_CORE_ASSERT( index < m_ColourAttachments.size() );
		return m_ColourAttachments[ index ];
	}


	Graphics::FramebufferSpecification const & OpenGLFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}

}

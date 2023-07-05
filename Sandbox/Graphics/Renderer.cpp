#include "Renderer.h"

#include "../Utils/Logger.h"
#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"

#include "../Editor/EditorUtilities.h"
#include "../Imgui/imgui.h"

#include "../Input/InputStatsComponent.h"

#include "TextureComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "GraphicsTags.h"
#include "CameraSystem.h"

#include "GraphicsDraw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <algorithm>

namespace Graphics
{
	Renderer::Renderer( Core::Registry & Registry, int width, int height )
		: m_registry( Registry )
		, m_screenDimensions { width, height }
	{
		RenderStats renderStats;
		renderStats.ScreenDimensions = m_screenDimensions;
		renderStats.FrameBufferDimensions = m_frameBufferDimensons;

		m_registry.ctx().emplace<RenderStats>( renderStats );

		GraphicsDraw::Initalize( this );
	}

	ShaderComponent Renderer::CreateShader( const std::string & name, Utils::EnumBitmask<Shader::Type> flags )
	{
		unsigned int ReturnId = 0;
		//duplicate and valid
		std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>( name.c_str(), flags );
		ReturnId = shader->getId();

		m_shaderList.push_back( shader );

		ShaderComponent shaderComponent { ReturnId, std::weak_ptr<IShaderHandle>( shader ) };
		return shaderComponent;
	}

	unsigned int Renderer::CreateTexture( const std::string & textureName )
	{
		unsigned int ReturnId = 0;
		//duplicate and valid
		Graphics::TexturePtr texture = m_textureLoader.getTexture( textureName.c_str() );
		ReturnId = texture->GetTextureID();

		return ReturnId;
	}

	MeshComponent Renderer::GetMesh( PrimativeType type )
	{
		MeshComponent comp;
		comp.Type = type;
		comp.MeshId = static_cast< MeshIdType >( type );
		return comp;
	}

	void Renderer::Init()
	{
		//Line drawing and line shader setup
		{
			CreateShader( "line", { Shader::Type::Vertex, Shader::Type::Fragment } );

			m_lineShader = m_shaderList.back().get();

			m_lineVao = VertexArrayObject::Create();
			m_lineVao->Bind();

			m_lineVbo = VertexBuffer::Create( m_maxLinesPerRender * GetElementSize( ShaderDataType::Float3 ) );
			m_lineVbo->SetLayout(
				{
					BufferElement::Create( ShaderDataType::Float3, "aPos" ),
					BufferElement::Create( ShaderDataType::Float4, "aColor" ),
					BufferElement::Create( ShaderDataType::Float, "aThickness" ) //unused for now but maybe later
				} );

			m_lineVao->AddVertexBuffer( m_lineVbo );

			m_lineDrawBegin = m_lineDrawCallList.begin();
			m_lineDrawItr = m_lineDrawBegin;
		}

		//Framebuffer setup
		{
			FramebufferSpecification framebufferSpecification;
			framebufferSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			framebufferSpecification.Width = m_screenDimensions.x*2;
			framebufferSpecification.Height = m_screenDimensions.y*2;
			m_Framebuffer = Framebuffer::Create( framebufferSpecification );

			ScreenQuadVAO = VertexArrayObject::Create();
			//vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			static constexpr float QuadVertices[] = {
				// positions		// texCoords
				-1.0f,  1.0f, 0.f,  0.0f, 1.0f,
				-1.0f, -1.0f, 0.f,  0.0f, 0.0f,
				 1.0f, -1.0f, 0.f,  1.0f, 0.0f,

				-1.0f,  1.0f, 0.f,  0.0f, 1.0f,
				 1.0f, -1.0f, 0.f,  1.0f, 0.0f,
				 1.0f,  1.0f, 0.f,  1.0f, 1.0f
			};

			ScreenQuadVBO = VertexBuffer::Create( QuadVertices, sizeof( QuadVertices ) );
			ScreenQuadVBO->SetLayout( {
				BufferElement::Create( ShaderDataType::Float3, "aPos" ),
				BufferElement::Create( ShaderDataType::Float2, "aTexCoords" ),
				} );
			ScreenQuadVAO->AddVertexBuffer( ScreenQuadVBO );

			CreateShader( "framebuffer_screen", { Shader::Type::Vertex, Shader::Type::Fragment } );
			m_screenShader = m_shaderList.back().get();
			m_screenShader->useProgram();
			m_screenShader->setUniform( "screenTexture", 0 );
			glUseProgram( 0 );
		}

		m_cameraUBO = UniformBufferObject::Create( sizeof( CameraData ), 0 );
	}

	void Renderer::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );

		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Graphics" ) )
			{
				ImGui::Checkbox( "Textures", &m_showTextureWindow );
				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Editor" ) )
			{
				if ( ImGui::Checkbox( "Render Scene into window", &m_useFrameBufferObject ) )
				{
					float const HeightToUse = static_cast< float >( m_useFrameBufferObject ? m_frameBufferDimensons.y : m_screenDimensions.y );
					float const WidthToUse = static_cast< float >( m_useFrameBufferObject ? m_frameBufferDimensons.x : m_screenDimensions.x );

					m_registry.ForView<CameraComponent, ActiveCameraTag>( [&HeightToUse, &WidthToUse] ( CameraComponent & cameraComponent )
						{
							cameraComponent.m_viewPortHeight = static_cast< float >( HeightToUse );
							cameraComponent.m_viewPortWidth = static_cast< float >( WidthToUse );
							cameraComponent.m_aspectRatio = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );
							cameraComponent.m_dirtyProjectionMatrix = true;
							cameraComponent.m_dirtyViewMatrix = true;
						} );

					//Resize the render texture buffer
					ResizeRenderTextureBuffer( WidthToUse, HeightToUse );
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if ( m_useFrameBufferObject )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
			ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
			if ( ImGui::Begin( "Game" ) )
			{
				// Using a Child allow to fill all the space of the window.
				// It also alows customization
				ImGui::BeginChild( "GameRender" );
				// Get the size of the child (i.e. the whole draw size of the windows).
				ImVec2 const wsize = ImGui::GetWindowSize();
				bool windowSizeChanged = false;
				windowSizeChanged = m_frameBufferDimensons.x != wsize.x;
				windowSizeChanged |= m_frameBufferDimensons.y != wsize.y;
				if ( windowSizeChanged )
				{
					m_frameBufferDimensons = { wsize.x, wsize.y };
				}
				ImVec2 const gameRenderPosition = ImGui::GetCursorScreenPos();
				m_frameBufferStartingPosition = { gameRenderPosition.x, gameRenderPosition.y };
				// Because I use the texture from OpenGL, I need to invert the V from the UV.
				auto textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image( static_cast< ImTextureID >( textureID ), { static_cast< float >( m_frameBufferDimensons.x ), static_cast< float >( m_frameBufferDimensons.y ) }, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

				ImVec2 const itemRectSize = ImGui::GetItemRectSize();

				RenderStats & renderStats = m_registry.ctx().get<RenderStats>();
				renderStats.FramebufferStartingPosition = { gameRenderPosition.x ,gameRenderPosition.y };
				renderStats.FrameBufferDimensions = { itemRectSize.x, itemRectSize.y };

				renderStats.ViewportDimensions = renderStats.FrameBufferDimensions;
				renderStats.ViewportStartingPosition = renderStats.FramebufferStartingPosition;

				renderStats.m_renderingSubwindow = true;

				if ( ImGui::IsItemHovered() )
				{
					Input::InputStatsComponent & inputStats = m_registry.ctx().get<Input::InputStatsComponent>();
					ImVec2 const imguiIOMousePos = ImGui::GetMousePos();
					inputStats.ViewPortCursorPos = { imguiIOMousePos.x - gameRenderPosition.x, imguiIOMousePos.y - gameRenderPosition.y };
				}

				ImGui::EndChild();

				if ( windowSizeChanged )
				{
					m_registry.ForView<CameraComponent, ActiveCameraTag>( [this] ( CameraComponent & cameraComponent )
						{
							cameraComponent.m_viewPortHeight = static_cast< float >( m_frameBufferDimensons.y );
							cameraComponent.m_viewPortWidth = static_cast< float >( m_frameBufferDimensons.x );
							cameraComponent.m_aspectRatio = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );
							cameraComponent.m_dirtyProjectionMatrix = true;
							cameraComponent.m_dirtyViewMatrix = true;
						} );

					//Resize the render texture buffer

					ResizeRenderTextureBuffer( static_cast< float >( m_frameBufferDimensons.x ), static_cast< float >( m_frameBufferDimensons.y ) );
				}
			}
			ImGui::End();
			ImGui::PopStyleVar( 3 );
		}
		else
		{
			RenderStats & renderStats = m_registry.ctx().get<RenderStats>();

			renderStats.ViewportDimensions = m_screenDimensions;
			renderStats.ViewportStartingPosition = { 0,0 }; //glfw's cursor position is from the top of the window

			renderStats.m_renderingSubwindow = false;
		}

		if ( m_showTextureWindow )
		{
			if ( ImGui::Begin( "Graphics" ) )
			{
				Graphics::TextureLoader::TextureMap const & textures = m_textureLoader.getLoadedTextures();

				//static ImGuiComboFlags flags = 0;
				//ImGui::CheckboxFlags( "ImGuiComboFlags_PopupAlignLeft", ( unsigned int * ) &flags, ImGuiComboFlags_PopupAlignLeft );
				//if( ImGui::CheckboxFlags( "ImGuiComboFlags_NoArrowButton", ( unsigned int * ) &flags, ImGuiComboFlags_NoArrowButton ) )
				//    flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
				//if( ImGui::CheckboxFlags( "ImGuiComboFlags_NoPreview", ( unsigned int * ) &flags, ImGuiComboFlags_NoPreview ) )
				//    flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

				std::vector<std::string> names;
				for ( auto & tex : textures )
				{
					names.push_back( tex.first );
				}
				const size_t textureCount = names.size();

				static unsigned int item_current_idx = 0; // Here our selection data is an index.
				std::string SelectedTexture = "no_texture_loaded";
				if ( ImGui::BeginCombo( "Textures", SelectedTexture.c_str()/*, flags*/ ) )
				{
					for ( unsigned int n = 0; n < names.size(); n++ )
					{
						bool const is_selected = ( item_current_idx == n );
						if ( ImGui::Selectable( names[ n ].c_str(), is_selected ) )
						{
							item_current_idx = n;
							SelectedTexture = names[ item_current_idx ];
						}

						if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if ( textureCount > 0 )
				{
					auto const texIt = textures.find( names[ item_current_idx ] );
					if ( texIt != textures.end() )
					{
						Texture const tex = texIt->second;
						//glActiveTexture(GL_TEXTURE0);
						//glBindTexture(GL_TEXTURE_2D, tex.GetTextureID());
						ImGui::Text( "pointer = %p", tex.GetTextureID() );
						ImGui::Text( "size = %d x %d", tex.GetWidth(), tex.GetHeight() );
						ImGui::Image( static_cast< ImTextureID >( tex.GetTextureID() ), ImVec2( 64, 64 ) );
					}
				}
			}
			ImGui::End();
		}

	}

	void Renderer::Render()
	{
		glUseProgram( 0 );

		glEnable( GL_DEPTH_TEST );

		glDisable( GL_BLEND );
		glDisable( GL_DITHER );
		//Render into framebuffer! 
		if ( m_firstTimeRenderingSubWindow )
		{
			m_registry.ForView<CameraComponent, ActiveCameraTag>( [this] ( CameraComponent & cameraComponent )
				{
					cameraComponent.m_viewPortHeight = static_cast< float >( m_frameBufferDimensons.y );
					cameraComponent.m_viewPortWidth = static_cast< float >( m_frameBufferDimensons.x );
					cameraComponent.m_aspectRatio = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );
					cameraComponent.m_dirtyProjectionMatrix = true;
					cameraComponent.m_dirtyViewMatrix = true;
				} );

			CameraSystem::InternalCameraUpdate( m_registry );
			m_firstTimeRenderingSubWindow = false;
		}

		glViewport( 0, 0, static_cast< GLsizei >( m_frameBufferDimensons.x ), static_cast< GLsizei >( m_frameBufferDimensons.y ) );
		m_Framebuffer->Bind();

		glClearColor( m_clearColour.x, m_clearColour.g, m_clearColour.b, m_clearColour.a );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//Grab the components from the registry here!
		m_registry.ForView<TransformComponent, MeshComponent, ShaderComponent, TextureComponent>(
			[this] ( TransformComponent & transform, MeshComponent & meshComponent, ShaderComponent & shaderComponent, TextureComponent & textureComponent )
			{
				UpdateShader( shaderComponent );

				UpdateCamera();

				UpdateTextures( textureComponent );

				UpdateTransform( transform );

				UpdateMesh( meshComponent );

				VertexArrayObject const * VAO = nullptr;

				//Render
				switch ( m_activeMeshType )
				{
					case Graphics::PrimativeType::Square:
						VAO = m_squareMesh.GetVAO().get();
						break;
					case Graphics::PrimativeType::Triangle:
						VAO = m_triangleMesh.GetVAO().get();
						break;
					case Graphics::PrimativeType::Cube:
						VAO = m_cubeMesh.GetVAO().get();
						break;
					default:
						break;
				}

				if ( VAO )
				{
					if ( VAO->HasIndicies() )
					{
						auto const indexBuffer = VAO->GetIndexBuffer();
						uint32_t const count = indexBuffer->GetCount();
						glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_INT, 0 );
					}
					else
					{
						//Hard case at the moment because cube isn't setup with indices yet
						auto const Buffer = VAO->GetVertexBuffers().front();
						uint32_t const count = Buffer->GetVertexCount();
						glDrawArrays( GL_TRIANGLES, 0, count );
					}
				}

			} );

		if ( m_lineDrawCallCount > 0 )
		{
			m_lineShader->useProgram();
			m_lineShader->setUniform( "model", glm::mat4( 1.f ) );

			//Update the buffer
			uint32_t const dataSize = static_cast< uint32_t >( reinterpret_cast< uint8_t * >( &( *m_lineDrawItr ) ) - reinterpret_cast< uint8_t * >( &( *m_lineDrawBegin ) ) );
			m_lineVbo->SetData( &( *m_lineDrawBegin ), dataSize );

			m_lineVao->Bind();
			for ( unsigned int i = 0; i <= m_lineDrawCallCount; ++i )
			{
				LineVertex const & call = m_lineDrawCallList[ i ];
				glLineWidth( call.Thickness );
				glDrawArrays( GL_LINES, i * 2, 2 );
			}
			m_lineVao->UnBind();

			m_lineDrawCallCount = 0;
			m_lineDrawItr = m_lineDrawBegin;

			m_activeShader->useProgram();
		}

		//Done rendering into frame buffer!
		m_Framebuffer->Unbind();

		glViewport( 0, 0, m_screenDimensions.x, m_screenDimensions.y );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glEnable( GL_BLEND );
		glBlendEquation( GL_FUNC_ADD );
		glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );

		m_screenShader->useProgram();
		glBindTextureUnit( 0, m_Framebuffer->GetColorAttachmentRendererID() );
		ScreenQuadVAO->Bind();
		glDrawArrays( GL_TRIANGLES, 0, 6 );
	}

	bool Renderer::DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour, float thickness ) noexcept
	{
		if ( m_lineDrawCallCount + 2 <= m_maxLinesPerRender )
		{
			m_lineDrawItr->Position = A;
			m_lineDrawItr->Colour = colour;
			m_lineDrawItr->Thickness = thickness;
			m_lineDrawItr++;

			m_lineDrawItr->Position = B;
			m_lineDrawItr->Colour = colour;
			m_lineDrawItr->Thickness = thickness;
			m_lineDrawItr++;

			m_lineDrawCallCount += 2;
			return true;
		}
		return false;
	}

	void Renderer::FramebufferSizeCallback( int width, int height )
	{
		m_frameBufferDimensons = { width, height };

		RenderStats & renderStats = m_registry.ctx().get<RenderStats>();
		renderStats.FrameBufferDimensions = { width, height };

		if ( m_useFrameBufferObject )
		{
			renderStats.ViewportDimensions = m_frameBufferDimensons;

			m_registry.ForView<CameraComponent, ActiveCameraTag>( [this] ( CameraComponent & cameraComponent )
				{
					cameraComponent.m_viewPortWidth = static_cast< float >( m_frameBufferDimensons.x );
					cameraComponent.m_viewPortHeight = static_cast< float >( m_frameBufferDimensons.y );
					cameraComponent.m_aspectRatio = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );

					cameraComponent.m_dirtyViewMatrix = true;
					cameraComponent.m_dirtyProjectionMatrix = true;
				} );
		}
	}

	void Renderer::ScreenSizeCallback( int width, int height )
	{
		m_screenDimensions = { width, height };

		RenderStats & renderStats = m_registry.ctx().get<RenderStats>();
		renderStats.ScreenDimensions = { width, height };

		if ( !m_useFrameBufferObject )
		{
			renderStats.ViewportDimensions = m_screenDimensions;

			m_registry.ForView<CameraComponent, ActiveCameraTag>( [this] ( CameraComponent & cameraComponent )
				{
					cameraComponent.m_viewPortWidth = static_cast< float >( m_screenDimensions.x );
					cameraComponent.m_viewPortHeight = static_cast< float >( m_screenDimensions.y );
					cameraComponent.m_aspectRatio = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );

					cameraComponent.m_dirtyViewMatrix = true;
					cameraComponent.m_dirtyProjectionMatrix = true;
				} );
		}

	}

	void Renderer::UpdateCamera()
	{
		//Camera
		m_registry.ForView<CameraComponent, ActiveCameraTag>( [this] ( CameraComponent & cameraComponent )
			{
				//Temp commenting out the dirty flag checks because when I use 2 different shaders and update the uniforms
				//It only works across 1 shader and then clears the flag
				//This will be fixed with the glsl global uniform block thingy.

				//if ( cameraComponent.m_dirtyProjectionMatrix )
				{
					cameraComponent.m_dirtyProjectionMatrix = false;
					//m_activeShader->setUniform( "projection", cameraComponent.m_projectionMatrix );
				}
				//if ( cameraComponent.m_dirtyViewMatrix )
				{
					cameraComponent.m_dirtyViewMatrix = false;
					//m_activeShader->setUniform( "view", cameraComponent.m_viewMatrix );
				}

				m_cameraData.ViewProjectionMatrix = cameraComponent.m_projectionMatrix * cameraComponent.m_viewMatrix;
				m_cameraUBO->SetData( &m_cameraData, sizeof( CameraData ) );
			} );
	}

	void Renderer::UpdateTransform( TransformComponent & transform )
	{
		//Build matrices
		glm::mat4 const translation = glm::translate( glm::mat4( 1.0f ), transform.Position );
		glm::mat4 const rotationMatrix = glm::toMat4( transform.RotationQuat );
		glm::mat4 const scale = glm::scale( glm::mat4( 1.f ), transform.Scale );
		glm::mat4 const transformation = translation * rotationMatrix * scale;
		m_activeShader->setUniform( "model", transformation );
	}

	void Renderer::UpdateTextures( TextureComponent & textureComponent )
	{
		std::vector<GLuint> const & textures = textureComponent.TextureList;
		size_t const newTexturesSize = textures.size();

		//unbind all textures
		for ( size_t i = 0; i < m_boundTexturesSize; ++i )
		{
			glBindTextureUnit( static_cast< GLuint >( i ), 0 );
		}

		//bind textures
		for ( size_t i = 0; i < newTexturesSize; ++i )
		{
			glBindTextureUnit( static_cast< GLuint >( i ), textures[ i ] );
		}
		m_boundTexturesSize = newTexturesSize;
		m_boundTextures = textures;
	}

	void Renderer::UpdateShader( ShaderComponent & shaderComponent )
	{
		//Getting active shader
		bool lookForShader = true;
		if ( m_activeShader )
		{
			lookForShader = m_activeShader->getId() != shaderComponent.ShaderId;
		}

		if ( lookForShader )
		{
			for ( auto & shaderPtr : m_shaderList )
			{
				if ( shaderComponent.ShaderId == shaderPtr->getId() )
				{
					m_activeShader = shaderPtr.get();
					break;
				}
			}
		}

		if ( m_activeShader )
		{
			//Bind Shader
			m_activeShader->useProgram();

			//#TODO, Bind uniforms based on what is within the shader, we can get a list of the shader uniforms from functions defined in the shader program.
			auto const & uniforms = m_activeShader->GetUniforms();
			//if ( uniforms.find( "texture1" ) != uniforms.end() )
			{
				m_activeShader->setUniform( "texture1", 0 );
			}
			//if ( uniforms.find( "texture2" ) != uniforms.end() )
			{
				m_activeShader->setUniform( "texture2", 1 );
			}
		}

	}

	void Renderer::UpdateMesh( MeshComponent & meshComponent )
	{
		//Mesh
		//if( meshComponent.type != m_activeMeshType )
		{
			switch ( meshComponent.Type )
			{
				case Graphics::PrimativeType::Square:
					m_squareMesh.Bind();
					break;
				case Graphics::PrimativeType::Triangle:
					m_triangleMesh.Bind();
					break;
				case Graphics::PrimativeType::Cube:
					m_cubeMesh.Bind();
					break;
				default:
					break;
			}
			m_activeMeshType = meshComponent.Type;
		}
	}

	void Renderer::ResizeRenderTextureBuffer( float WidthToUse, float HeightToUse )
	{
		m_Framebuffer->Resize( static_cast< uint32_t >( WidthToUse*2 ), static_cast< uint32_t >( HeightToUse*2 ) );
	}

}

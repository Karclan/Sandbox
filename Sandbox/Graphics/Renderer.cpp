#include "Renderer.h"

#include "../Utils/Logger.h"
#include "../Utils/UnusedMacro.h"
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <algorithm>



namespace Graphics
{
	//vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	static constexpr float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;
	void CreateScreenQuad()
	{
		glGenVertexArrays( 1, &quadVAO );
		glGenBuffers( 1, &quadVBO );
		glBindVertexArray( quadVAO );
		glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), &quadVertices, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), ( void * ) 0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), ( void * ) ( 2 * sizeof( float ) ) );
	}

	Renderer::Renderer( Core::Registry & Registry, int width, int height )
		: m_registry( Registry )
		, m_screenDimensions { width, height }
	{
		RenderStats renderStats;
		renderStats.ScreenDimensions = m_screenDimensions;
		renderStats.FrameBufferDimensions = m_frameBufferDimensons;

		m_registry.ctx().emplace<RenderStats>( renderStats );
	}

	ShaderComponent Renderer::CreateShader( const std::string & name, Utils::EnumBitmask<Shader::Type> flags )
	{
		unsigned int ReturnId = 0;
		//duplicate and valid
		auto shader = std::make_shared<ShaderProgram>( name.c_str(), flags );
		ReturnId = shader->getId();

		m_shaderList.push_back( shader );

		ShaderComponent shaderComponent { ReturnId, std::weak_ptr<IShaderHandle>( shader ) };
		return shaderComponent;
	}

	unsigned int Renderer::CreateTexture( const std::string & textureName )
	{
		unsigned int ReturnId = 0;
		//duplicate and valid
		auto texture = m_textureLoader.getTexture( textureName.c_str() );
		ReturnId = texture->GetTextureID();

		return ReturnId;
	}

	MeshComponent Renderer::GetMesh( PrimativeType type )
	{
		MeshComponent comp;
		comp.type = type;
		comp.MeshId = static_cast< unsigned int >( type );
		return comp;
	}

	void Renderer::Init()
	{
		{
			CreateShader( "line", { Shader::Type::Vertex, Shader::Type::Fragment } );

			m_lineShader = m_shaderList.back().get();
			//bindVAO();
			if ( m_lineVao == 0 )
			{
				glGenVertexArrays( 1, &m_lineVao );
			}

			glBindVertexArray( m_lineVao );
			//bindVertices( vertices, sizeof( vertices ), 8 );
			if ( m_lineVbo == 0 )
			{
				glGenBuffers( 1, &m_lineVbo );
			}
			glBindBuffer( GL_ARRAY_BUFFER, m_lineVbo );
			glBufferData( GL_ARRAY_BUFFER, m_maxLinesPerRender * 3, 0, GL_DYNAMIC_DRAW );

			//Vertex points
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void * ) 0 );
			glEnableVertexAttribArray( 0 );

			//finishBinding();
			glBindBuffer( GL_ARRAY_BUFFER, 0 );
			glBindVertexArray( 0 );
		}


		CreateRenderBufferObject();
		{
			CreateShader( "framebuffer_screen", { Shader::Type::Vertex, Shader::Type::Fragment } );
			m_screenShader = m_shaderList.back().get();
			m_screenShader->useProgram();
			m_screenShader->setUniform( "screenTexture", 0 );
			glUseProgram( 0 );
		}
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
					float HeightToUse = static_cast< float >( m_useFrameBufferObject ? m_frameBufferDimensons.y : m_screenDimensions.y );
					float WidthToUse = static_cast< float >( m_useFrameBufferObject ? m_frameBufferDimensons.x : m_screenDimensions.x );

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
				ImGui::Image( static_cast< ImTextureID >( m_renderedTexture ), { static_cast< float >( m_frameBufferDimensons.x ), static_cast< float >( m_frameBufferDimensons.y ) }, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

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
			renderStats.ViewportStartingPosition = {0,0}; //glfw's cursor position is from the top of the window

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
				for ( auto tex : textures )
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
					auto texIt = textures.find( names[ item_current_idx ] );
					if ( texIt != textures.end() )
					{
						Texture tex = texIt->second;
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
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );
		glBlendEquation( GL_FUNC_ADD );
		glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );

		if ( m_useFrameBufferObject )
		{
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

			glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObject );
			glViewport( 0, 0, static_cast< GLsizei >( m_frameBufferDimensons.x ), static_cast< GLsizei >( m_frameBufferDimensons.y ) );
		}

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//Grab the components from the registry here!
		m_registry.ForView<TransformComponent, MeshComponent, ShaderComponent, TextureComponent>(
			[this] ( TransformComponent & transform, MeshComponent & meshComponent, ShaderComponent & shaderComponent, TextureComponent & textureComponent )
			{
				UpdateShader( shaderComponent );

				UpdateCamera();

				UpdateTextures( textureComponent );

				UpdateTransform( transform );

				//m_activeShader->setUniform( "colourMod", glm::vec4( 1.f ) );

				UpdateMesh( meshComponent );

				//Render
				switch ( m_activeMeshType )
				{
					case Graphics::PrimativeType::Square:
						glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
						break;
					case Graphics::PrimativeType::Triangle:
						glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );
						break;
					case Graphics::PrimativeType::Cube:
						glDrawArrays( GL_TRIANGLES, 0, 36 );
						break;
					default:
						break;
				}
			} );

		if ( m_lineDrawCalls > 0 )
		{
			m_lineShader->useProgram();


			m_lineShader->setUniform( "model", glm::mat4( 1.f ) );
			m_registry.ForConstView<CameraComponent const, ActiveCameraTag const>(
				[this] ( CameraComponent const & cameraComponent )
				{
					m_lineShader->setUniform( "projection", cameraComponent.m_projectionMatrix );
					m_lineShader->setUniform( "view", cameraComponent.m_viewMatrix );
				} );
			glBindVertexArray( m_lineVao );
			for ( unsigned int i = 0; i <= m_lineDrawCalls; ++i )
			{
				auto & call = m_lineDrawCallList[ i ];
				call.m_colour;
				m_lineShader->setUniform( "colour", call.m_colour );
				glLineWidth( call.m_lineThickness );
				glDrawArrays( GL_LINE_STRIP, i * 2, 2 );
			}
			glBindVertexArray( 0 );
			m_lineDrawCalls = 0;
			m_activeShader->useProgram();
		}

		if ( m_useFrameBufferObject )
		{
			glViewport( 0, 0, m_frameBufferDimensons.x, m_frameBufferDimensons.y );
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );

			glClearColor( 0.f, 0.f, 0.f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			/*m_screenShader->useProgram();
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);*/

			//glBindVertexArray(0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	bool Renderer::DrawLine( glm::vec3 A, glm::vec3 B, glm::vec4 colour, float thickness ) noexcept
	{
		//Add two points into the buffer
		if ( m_lineDrawCalls + 1 <= m_maxLinesPerRender )
		{
			++m_lineDrawCalls;
			int constexpr vecSize = sizeof( glm::vec3 );
			int const offset1 = ( m_lineDrawCalls * ( vecSize * 2 ) );
			int const offset2 = ( offset1 + vecSize );
			glBindVertexArray( m_lineVao );
			glBindBuffer( GL_ARRAY_BUFFER, m_lineVbo );
			glBufferSubData( GL_ARRAY_BUFFER, offset1, vecSize, &A );
			glBufferSubData( GL_ARRAY_BUFFER, offset2, vecSize, &B );
			glBindBuffer( GL_ARRAY_BUFFER, 0 );
			glBindVertexArray( 0 );
			auto & call = m_lineDrawCallList[ m_lineDrawCalls ];
			call.PointA = A;
			call.PointB = B;
			call.m_colour = colour;
			call.m_lineThickness = thickness;
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
				if ( cameraComponent.m_dirtyProjectionMatrix )
				{
					cameraComponent.m_dirtyProjectionMatrix = false;
					m_activeShader->setUniform( "projection", cameraComponent.m_projectionMatrix );
				}
				if ( cameraComponent.m_dirtyViewMatrix )
				{
					cameraComponent.m_dirtyViewMatrix = false;
					m_activeShader->setUniform( "view", cameraComponent.m_viewMatrix );
				}
			} );
	}

	void Renderer::UpdateTransform( TransformComponent & transform )
	{
		//Build matrices

		//glm::mat4 transformation = glm::mat4( 1.f );
		//transformation = glm::translate( transformation, transform.Position );
		//glm::mat4 rotationMatrix = glm::toMat4( transform.RotationQuat );
		//transformation = transformation * rotationMatrix;
		//transformation = glm::scale( transformation, transform.Scale );

		glm::mat4 translation = glm::translate( glm::mat4( 1.0f ), transform.Position );
		glm::mat4 rotationMatrix = glm::toMat4( transform.RotationQuat );
		glm::mat4 scale = glm::scale( glm::mat4( 1.f ), transform.Scale );
		glm::mat4 transformation = translation * rotationMatrix * scale;
		m_activeShader->setUniform( "model", transformation );
	}

	void Renderer::UpdateTextures( TextureComponent & textureComponent )
	{
		bool rebindTextures = false;
		std::vector<GLuint> const & textures = textureComponent.TextureList;
		size_t const newTexturesSize = textures.size();
		if ( newTexturesSize == m_boundTexturesSize )
		{
			for ( size_t i = 0; i < newTexturesSize; ++i )
			{
				if ( textures[ i ] != m_boundTextures[ i ] )
				{
					rebindTextures = true;
					break;
				}
			}
		}
		else
		{
			size_t const diff = m_boundTexturesSize - newTexturesSize;
			if ( diff > 0 ) //We have less textures than last
			{
				for ( int i = static_cast< int >( m_boundTexturesSize ); i > -1; --i ) //Go down the list and unbind textures
				{
					//unbind
					glActiveTexture( GL_TEXTURE0 + static_cast< GLenum >( i ) );
					glBindTexture( GL_TEXTURE_2D, 0 );
				}
			}
			m_boundTexturesSize = newTexturesSize;
			rebindTextures = true;
		}

		if ( rebindTextures )
		{
			for ( size_t i = 0; i <= m_boundTexturesSize; ++i )
			{
				glActiveTexture( GL_TEXTURE0 + static_cast< GLenum >( i ) );
				glBindTexture( GL_TEXTURE_2D, 0 );
			}
			for ( size_t i = 0; i < textures.size(); ++i )
			{
				//bind textures
				glActiveTexture( GL_TEXTURE0 + static_cast< GLenum >( i ) );
				glBindTexture( GL_TEXTURE_2D, textures[ i ] );
			}
			m_boundTextures = textures;
		}
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
				}
			}
		}

		if ( m_activeShader )
		{
			//Bind Shader
			m_activeShader->useProgram();

			//#TODO, Bind uniforms based on what is within the shader, we can get a list of the shader uniforms from functions defined in the shader program.
			m_activeShader->setUniform( "texture1", 0 );
			m_activeShader->setUniform( "texture2", 1 );
		}

	}

	void Renderer::UpdateMesh( MeshComponent & meshComponent )
	{
		//Mesh
		//if( meshComponent.type != m_activeMeshType )
		{
			switch ( meshComponent.type )
			{
				case Graphics::PrimativeType::Square:
					m_squareMesh.bind();
					break;
				case Graphics::PrimativeType::Triangle:
					m_triangleMesh.bind();
					break;
				case Graphics::PrimativeType::Cube:
					m_cubeMesh.bind();
					break;
				default:
					break;
			}
			m_activeMeshType = meshComponent.type;
		}
	}

	void Renderer::CreateRenderBufferObject()
	{
		//Create frame buffer object
		glGenFramebuffers( 1, &m_frameBufferObject );
		glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObject );


		//Create texture for it
		glGenTextures( 1, &m_renderedTexture );
		glBindTexture( GL_TEXTURE_2D, m_renderedTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_frameBufferDimensons.x, m_frameBufferDimensons.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glBindTexture( GL_TEXTURE_2D, 0 );

		//Bind texture to frame buffer
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderedTexture, 0 );

		//Create and bind render buffer object
		glGenRenderbuffers( 1, &m_renderBufferObject );
		glBindRenderbuffer( GL_RENDERBUFFER, m_renderBufferObject );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_frameBufferDimensons.x, m_frameBufferDimensons.y );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		{
			Utils::Logger::LogMessage( Utils::LogType::Error, "Framebuffer is not complete!\n" );
		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void Renderer::ResizeRenderTextureBuffer( float WidthToUse, float HeightToUse )
	{
		//Resize the render texture buffer
		glBindTexture( GL_TEXTURE_2D, m_renderedTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, static_cast< GLsizei >( WidthToUse ), static_cast< GLsizei >( HeightToUse ), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glBindRenderbuffer( GL_RENDERBUFFER, m_renderBufferObject );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast< GLsizei >( WidthToUse ), static_cast< GLsizei >( HeightToUse ) );

		m_boundTexturesSize = 0;
	}

}

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

#include "../magic_enum/magic_enum.hpp"

#include <algorithm>
#include <functional>
#include <locale>

namespace Graphics
{
	Renderer * s_RendererPtr = nullptr;
	void GLAPIENTRY glDebugOutput( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char * message, const void * userParam )
	{
		s_RendererPtr->GLDebugOutput( source, type, id, severity, length, message, userParam );
	}

	Renderer::Renderer( Core::Registry & Registry, int width, int height )
		: m_registry( Registry )
		, m_screenDimensions { width, height }
		, m_shaderResourceHandler()
	{
		RenderStats renderStats;
		renderStats.ScreenDimensions = m_screenDimensions;
		renderStats.FrameBufferDimensions = m_frameBufferDimensons;

		m_registry.ctx().emplace<RenderStats>( renderStats );

		s_RendererPtr = this;
		GraphicsDraw::Initalize( this );
		
		SB::Shared<Core::ResourceTypeHandler> shaderResourceHandler = std::make_shared<TShaderResourceHandler>();
		m_shaderResourceHandler = std::static_pointer_cast<TShaderResourceHandler>(shaderResourceHandler);

		Core::Resources::AddResourceHandler( shaderResourceHandler );
	}

	void Renderer::GLDebugOutput( GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		[[maybe_unused]] GLsizei length,
		const char * message,
		[[maybe_unused]] const void * userParam )
	{
		//#TODO_Shaders: Search message for shader program ID and instead add our shader name/sub program name into the string to help readabilitly
		static std::unordered_set<int> const ignoredDebugOutput = {
			131185 //131185 - Buffer detailed info
		};

		if ( ignoredDebugOutput.contains( id ) )
		{
			return; // ignore these non-significant error codes
		}

		if ( id == 131169 || id == 131218 || id == 131204 )
		{
			SB_CORE_WARN( "Previously ignored debugout {} discovered\n", id );
		}

		std::string openGLMessage = message;

		bool const bShaderMessage = openGLMessage.find( "Program/shader" ) != std::string::npos;
		if ( bShaderMessage )
		{
			static std::string const numberstring ( "0123456789" );

			size_t const numberLocation = openGLMessage.find_first_of(numberstring);
			size_t nextSpace = openGLMessage.find_first_of( " ", numberLocation );
			int programId = std::stoi( openGLMessage.substr( numberLocation, nextSpace - numberLocation ));
			int shaderId = -1;
			int subShaderId = -1;
			std::string ShaderName = "";
			std::string SubProgramName = "";

			auto const & shaderList = m_shaderResourceHandler->GetShaders();

			for ( auto const & shaderItr : shaderList )
			{
				auto const & shader = shaderItr.Resource;
				shaderId = shader->GetShaderId();
				ShaderName = shader->GetShaderName();
				if ( shaderId == programId )
				{
					break;
				}
				else
				{
					auto const & programs = shader->GetPrograms();
					for ( auto const & program : programs )
					{
						subShaderId = program.getId();
						if ( subShaderId == programId )
						{
							SubProgramName = magic_enum::enum_name<Shader::Type>(program.getType());
							break;
						}
					}
				}
			}
			std::string insertString;
			if ( SubProgramName.empty() )
			{
				insertString = " \"" + ShaderName + '\"';
			}
			else
			{
				insertString = " \"" + ShaderName + "\" - \"" + SubProgramName + '\"';
			}
			openGLMessage.insert( nextSpace, insertString );
		}

		std::string debugMessage = "--------------- \n";
		debugMessage.append( "Debug message (" );
		debugMessage.append( std::to_string( id ) );
		debugMessage.append( "): " );
		debugMessage.append( openGLMessage );
		debugMessage.append( "\n" );

		debugMessage.append( "Source :" );
		switch ( source )
		{
			case GL_DEBUG_SOURCE_API:
				debugMessage.append( "API\n" );
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				debugMessage.append( "System\n" );
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				debugMessage.append( "Compiler\n" );
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				debugMessage.append( "Third Party\n" );
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				debugMessage.append( "Application\n" );
				break;
			case GL_DEBUG_SOURCE_OTHER:
				debugMessage.append( "Other\n" );
				break;
			default:
				debugMessage.append( "Unknown source\n" );
				break;
		}

		debugMessage.append( "Type: " );
		switch ( type )
		{
			case GL_DEBUG_TYPE_ERROR:
				debugMessage.append( "Error\n" );
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				debugMessage.append( "Deprecated Behaviour\n" );
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				debugMessage.append( "Undefined Behaviour\n" );
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				debugMessage.append( "Portability\n" );
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				debugMessage.append( "Performance\n" );
				break;
			case GL_DEBUG_TYPE_MARKER:
				debugMessage.append( "Marker\n" );
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				debugMessage.append( "Push Group\n" );
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				debugMessage.append( "Pop Group\n" ); break;
			case GL_DEBUG_TYPE_OTHER:
				debugMessage.append( "Other\n" );
				break;
			default:
				debugMessage.append( "Unknown type\n" );
				break;
		}

		debugMessage.append( "Severity :" );
		switch ( severity )
		{
			case GL_DEBUG_SEVERITY_HIGH:
				debugMessage.append( "high\n" );
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				debugMessage.append( "medium\n" );
				break;
			case GL_DEBUG_SEVERITY_LOW:
				debugMessage.append( "low\n" );
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				debugMessage.append( "notification\n" );
				break;
			default:
				debugMessage.append( "Unknown severity\n" );
				break;
		}

		Utils::Logger::LogMessage( Utils::LogType::Error, debugMessage.c_str() );
		//SB_CORE_ASSERT(false);
	}

	//ShaderComponent Renderer::GetOrCreateShader( std::string const & name, Utils::EnumBitmask<Shader::Type> flags )
	//{
	//	ShaderComponent shaderComponent;
	//	if ( ShaderMap::const_iterator FoundShader = m_shaderList.find( name ); FoundShader != m_shaderList.end() )
	//	{
	//		shaderComponent = { FoundShader->second->GetShaderId(), std::weak_ptr<IShaderHandle>( FoundShader->second ) };
	//	}
	//	else
	//	{
	//		std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>( name.c_str(), flags );
	//		m_shaderList[name] = shader;

	//		shaderComponent = { shader->GetShaderId(), std::weak_ptr<IShaderHandle const>( shader ) };
	//	}
	//	return shaderComponent;
	//}

	unsigned int Renderer::GetOrCreateTexture( const std::string & textureName )
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
		// enable OpenGL debug context if context allows for debug context
		int flags = 0; 
		glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
		if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
		{
			glEnable( GL_DEBUG_OUTPUT );
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS ); // makes sure errors are displayed synchronously
			glDebugMessageCallback( glDebugOutput, NULL );
			glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE/*GL_DEBUG_SEVERITY_NOTIFICATION*/, 0, nullptr, GL_TRUE );
		}

		//Line drawing and line shader setup
		{
			m_lineShader = m_shaderResourceHandler->GetOrLoadResource("line");
			
			m_lineVao = VertexArrayObject::Create();
			m_lineVao->Bind();

			;
			m_lineVbo = VertexBuffer::Create( m_maxLinesPerRender * sizeof( LineVertex ) );
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

		{
			m_circleShader = m_shaderResourceHandler->GetOrLoadResource("wireCircle");

			m_circleVao = VertexArrayObject::Create();
			m_circleVao->Bind();

			m_circleVbo = VertexBuffer::Create( m_maxCirclesPerRender * sizeof( CircleVertex ) );
			m_circleVbo->SetLayout(
				{
					BufferElement::Create( ShaderDataType::Float3, "aPos" ),
					BufferElement::Create( ShaderDataType::Float4, "aColor" ),
					BufferElement::Create( ShaderDataType::Float, "aRadius" ),
					BufferElement::Create( ShaderDataType::Int, "aPointCount" ),
				} );

			m_circleVao->AddVertexBuffer( m_circleVbo );

			m_circleDrawBegin = m_circleDrawCallList.begin();
			m_circleDrawItr = m_circleDrawBegin;
		}

		//Framebuffer setup
		{
			FramebufferSpecification framebufferSpecification;
			framebufferSpecification.Attachments = { FramebufferTextureFormat::RGBA8, /*FramebufferTextureFormat::RED_INTEGER,*/ FramebufferTextureFormat::Depth };
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


			m_screenShader = m_shaderResourceHandler->GetOrLoadResource("framebuffer_screen");
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
		/*DrawCircle( glm::vec3( 0.f, 0.f, 0.f ), 15.f, Colours::White, 128 );
		DrawCircle( glm::vec3( 1.f, 1.f, 0.f ), 0.5f, Colours::Red, 3 );
		DrawCircle( glm::vec3( -1.f, -1.f, 0.f), 10.f, Colours::Green, 3 );
		DrawCircle( glm::vec3( 2.f, 2.f, 0.f), 1.f, Colours::Blue, 3 );*/

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

		if ( m_circleDrawCallCount > 0 )
		{
			m_circleShader->useProgram();
			m_circleShader->setUniform( "model", glm::mat4( 1.f ) );

			//Update the buffer
			uint32_t const dataSize = static_cast< uint32_t >( reinterpret_cast< uint8_t * >( &( *m_circleDrawItr ) ) - reinterpret_cast< uint8_t * >( &( *m_circleDrawBegin ) ) );
			m_circleVbo->SetData( &( *m_circleDrawBegin ), dataSize );
			  
			m_circleVao->Bind();
			for ( unsigned int i = 0; i <= m_circleDrawCallCount; ++i )
			{
				CircleVertex const & call = m_circleDrawCallList[ i ];
				glDrawArrays( GL_POINTS, i, 1 );
			}
			m_circleVao->UnBind();

			m_circleDrawCallCount = 0;
			m_circleDrawItr = m_circleDrawBegin;

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

	bool Renderer::DrawCircle( glm::vec3 pos, float radius, glm::vec4 colour /*= glm::vec4( 1.f )*/, int pointCount /*= 3.f */ ) noexcept
	{
		if ( m_circleDrawCallCount + 1 <= m_maxCirclesPerRender )
		{
			m_circleDrawItr->Position = pos;
			m_circleDrawItr->Colour = colour;
			m_circleDrawItr->Radius = radius;
			m_circleDrawItr->PointCount = pointCount;
			m_circleDrawItr++;

			++m_circleDrawCallCount;
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
				m_cameraData.ViewMatrix = cameraComponent.m_viewMatrix;
				m_cameraData.ProjectionMatrix = cameraComponent.m_projectionMatrix;
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
			lookForShader = m_activeShader->GetShaderId() != shaderComponent.ShaderId;
		}

		if ( lookForShader )
		{
			if (auto const & shader = m_shaderResourceHandler->GetShader(shaderComponent))
			{
				m_activeShader = shader;
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

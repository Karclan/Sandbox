#include "Basic2DGameTestSubsystem.h"

#include "Graphics/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/CameraSystem.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"
#include "magic_enum/magic_enum.hpp"

namespace Game
{
	//Need to create an ortho camera - Done
	//Need to create a sprite class which makes use of a rectangle

	Basic2DGameTestSubsystem::Basic2DGameTestSubsystem( Core::Registry & registryRef, Graphics::Window & window, Graphics::CameraSystem & cameraSystem )
		: Core::Subsystem( registryRef )
		, m_windowRef( window )
		, m_cameraSystemRef( cameraSystem )
	{
		Init();
	}

	void Basic2DGameTestSubsystem::Init()
	{
		auto & renderer = m_windowRef.GetRenderer();

		/*auto basicShaderId = renderer.CreateShader( "basic", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );*/
		//Create camera
		Core::Entity CameraEntity = m_cameraSystemRef.CreatePerspectiveCamera( "MainCamera" );
		m_cameraSystemRef.SetActiveCamera( "MainCamera" );


		//auto containerTextureId = renderer.CreateTexture( "container.jpg" );

		//Graphics::TextureComponent containerTextureComp { { containerTextureId } };
		//Graphics::ShaderComponent textureEnabledShaderComp { basicShaderId };
		//Graphics::TransformComponent triangleTransform;

		//triangleTransform.Position = { 1.f, 1.f, -1.f };

		//Core::Entity entity = Subsystem::m_registry.CreateEntity();
		//Subsystem::m_registry.AddComponent<Graphics::TransformComponent>( entity, triangleTransform );
		//Subsystem::m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
		//Subsystem::m_registry.AddComponent<Graphics::TextureComponent>( entity, containerTextureComp );
		//Subsystem::m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );

		//Component and entity creation/registration
		{
			using namespace magic_enum::bitwise_operators;
			//Move into system or at least out of here
			auto basicShaderId = renderer.CreateShader( "basic", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			auto colourShiftShaderId = renderer.CreateShader( "colourShift", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			auto textureEnabledShaderId = renderer.CreateShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			auto containerTextureId = renderer.CreateTexture( "container.jpg" );
			auto awesomefaceTextureId = renderer.CreateTexture( "awesomeface.png" );

			Graphics::TextureComponent containerTextureComp { { containerTextureId } };
			Graphics::TextureComponent awesomeFaceTextureComp { { awesomefaceTextureId } };
			Graphics::TextureComponent multiTextureComp { { containerTextureId, awesomefaceTextureId } };

			Graphics::ShaderComponent basicShaderComp { basicShaderId };
			Graphics::ShaderComponent colourShiftShaderComp { colourShiftShaderId };
			Graphics::ShaderComponent textureEnabledShaderComp { textureEnabledShaderId };

			//Triangle
			{
				Graphics::TransformComponent triangleTransform;
				triangleTransform.Position = { -2.f,-2.f,-1.f };
				triangleTransform.Scale = { .5f, .5f, .5f };

				auto entity = m_registry.CreateEntity();
				m_registry.AddComponent<Graphics::TransformComponent>( entity, triangleTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Triangle ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, containerTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
			}

			//Square
			{
				Graphics::TransformComponent squareTransform;
				squareTransform.Position = { 1.f, 1.f, 0.f };

				auto entity = m_registry.CreateEntity();
				m_registry.AddComponent<Graphics::TransformComponent>( entity, squareTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Square ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, awesomeFaceTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
			}

			//Cube
			{
				Graphics::TransformComponent cubeTransform;

				auto entity = m_registry.CreateEntity();
				m_registry.AddComponent<Graphics::TransformComponent>( entity, cubeTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
			}

			for ( int i = 0; i < 10; ++i )
			{
				{
					Graphics::TransformComponent randomTransform;
					randomTransform.Position = { i, i, i / 2.f };

					auto entity = m_registry.CreateEntity();
					m_registry.AddComponent<Graphics::TransformComponent>( entity, randomTransform );
					m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
					m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
					m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
				}
			}
		}
	}

	void Basic2DGameTestSubsystem::FixedUpdate( double deltaTime )
	{

	}

	void Basic2DGameTestSubsystem::Update( double deltaTime )
	{

	}

}
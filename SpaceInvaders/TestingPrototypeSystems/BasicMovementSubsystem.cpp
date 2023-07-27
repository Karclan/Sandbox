#include "BasicMovementSubsystem.h"

#include "Core/CoreRegistry.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"

#include "Generic/NameComponent.h"

#include "Utils/format.h"

#include "magic_enum/magic_enum.hpp"

#include <corecrt_math_defines.h>

#define TAU (M_PI * 2.0f)

namespace Game
{
	BasicMovementSubsystem::BasicMovementSubsystem( Core::Registry & m_registry )
		: Subsystem( m_registry )
	{
		m_entity = m_registry.CreateEntity( "Movement Structure Entity" );
		m_registry.AddComponent<MovementStruct>( m_entity, MovementStruct {} );
	}

	void BasicMovementSubsystem::CreateComponents( Graphics::Window & window )
	{
		//Component and entity creation/registration
		{
			Graphics::Renderer & renderer = window.GetRenderer();


			using namespace magic_enum::bitwise_operators;
			//Move into system or at least out of here
			Graphics::ShaderComponent basicShaderComp = renderer.GetOrCreateShader( "basic", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			Graphics::ShaderComponent colourShiftShaderComp = renderer.GetOrCreateShader( "colourShift", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			Graphics::ShaderComponent textureEnabledShaderComp = renderer.GetOrCreateShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
			auto containerTextureId = renderer.GetOrCreateTexture( "container.jpg" );
			auto awesomefaceTextureId = renderer.GetOrCreateTexture( "awesomeface.png" );

			Graphics::TextureComponent containerTextureComp { { containerTextureId } };
			Graphics::TextureComponent awesomeFaceTextureComp { { awesomefaceTextureId } };
			Graphics::TextureComponent multiTextureComp { { containerTextureId, awesomefaceTextureId } };

			//Triangle
			{
				Graphics::TransformComponent triangleTransform;
				triangleTransform.Position = { -2.f,-2.f,0.f };
				triangleTransform.Scale = { .5f, .5f, .5f };

				auto entity = m_registry.CreateEntity( "Triangle" );
				m_registry.AddComponent<Graphics::TransformComponent>( entity, triangleTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Triangle ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, containerTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
				m_registry.AddComponent<EntityTag>( entity );
			}

			//Square
			{
				Graphics::TransformComponent squareTransform;
				squareTransform.Position = { 1.f, 1.f, 0.f };

				auto entity = m_registry.CreateEntity( "Square" );
				m_registry.AddComponent<Graphics::TransformComponent>( entity, squareTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Square ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, awesomeFaceTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
				m_registry.AddComponent<EntityTag>( entity );
			}

			//Cube
			{
				Graphics::TransformComponent cubeTransform;

				auto entity = m_registry.CreateEntity( "Cube" );
				m_registry.AddComponent<Graphics::TransformComponent>( entity, cubeTransform );
				m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
				m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
				m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
				m_registry.AddComponent<EntityTag>( entity );
			}

			for ( int i = 0; i < 10; ++i )
			{
				{
					Graphics::TransformComponent randomTransform;
					randomTransform.Position = { i, i/2, 0.f };

					auto entity = m_registry.CreateEntity( fmt::format( "Cube-{}", i ) );
					m_registry.AddComponent<Graphics::TransformComponent>( entity, randomTransform );
					m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
					m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
					m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
					m_registry.AddComponent<EntityTag>( entity );
				}
			}
		}
	}

	void BasicMovementSubsystem::Integrate( double deltaTime )
	{
		MovementStruct & movementStruct = m_registry.GetComponent<MovementStruct>( m_entity );

		//Test movement code
		movementStruct.m_angle += movementStruct.speed * static_cast< float >( deltaTime );
		if ( movementStruct.m_angle > static_cast< float >( TAU ) ) 
		{ 
			movementStruct.m_angle -= static_cast< float >( TAU );
		}
		movementStruct.m_xy.x = glm::sin( movementStruct.m_angle );
		movementStruct.m_xy.y = glm::cos( movementStruct.m_angle );

		/*m_registry.ForView<Graphics::TransformComponent, Graphics::MeshComponent>(
			[&movementStruct, this] ( Core::Entity entity, Graphics::TransformComponent & transform, Graphics::MeshComponent & mesh )
			{
				bool hasTag = m_registry.HasComponent<EntityTag>( entity );
				if ( hasTag )
				{
					switch ( mesh.type )
					{
						case Graphics::PrimativeType::Cube:
							transform.Position.x = movementStruct.m_xy.x;
							transform.RotationQuat = glm::angleAxis( movementStruct.m_xy.x, Graphics::Constants::Right );
							transform.RotationQuat *= glm::angleAxis( movementStruct.m_xy.y, Graphics::Constants::Up );
							transform.RotationQuat *= glm::angleAxis( movementStruct.m_xy.x * movementStruct.m_xy.y, Graphics::Constants::Forward );
							break;
						case Graphics::PrimativeType::Triangle:
							transform.RotationQuat = glm::angleAxis( movementStruct.m_xy.x, Graphics::Constants::Right );
							transform.RotationQuat *= glm::angleAxis( movementStruct.m_xy.y, Graphics::Constants::Up );
							break;
						case Graphics::PrimativeType::Square:
							transform.Position = { movementStruct.m_xy.x ,movementStruct.m_xy.y, 1.f };
							break;
						default:
							break;
					}
				}
			} );*/

			/*   auto view = m_registry.View<Graphics::TransformComponent, Graphics::MeshComponent>();
			   for( auto const & entity : view )
			   {
				   Graphics::TransformComponent & transform = view.get<Graphics::TransformComponent>( entity );
				   Graphics::MeshComponent & mesh = view.get<Graphics::MeshComponent>( entity );

				   switch( mesh.type )
				   {
				   case Graphics::PrimativeType::Cube:
					   transform.Position.x = movementStruct.m_xy.x;
					   transform.RotationQuat = glm::angleAxis(movementStruct.m_xy.x, Graphics::Constants::Right);
					   transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.y, Graphics::Constants::Up);
					   transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.x * movementStruct.m_xy.y, Graphics::Constants::Forward);
					   break;
				   case Graphics::PrimativeType::Triangle:
					   transform.RotationQuat = glm::angleAxis(movementStruct.m_xy.x, Graphics::Constants::Right);
					   transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.y, Graphics::Constants::Up);
					   break;
				   case Graphics::PrimativeType::Square:
					   transform.Position = { movementStruct.m_xy.x ,movementStruct.m_xy.y, 1.f };
					   break;
				   default:
					   break;
				   }
			   }*/
	}

	void BasicMovementSubsystem::FixedUpdate( double deltaTime )noexcept
	{
		Integrate( deltaTime );
	}
}


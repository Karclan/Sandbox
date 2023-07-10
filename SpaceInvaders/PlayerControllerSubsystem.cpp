#include "PlayerControllerSubsystem.h"

#include "Utils/UnusedMacro.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"
#include "Graphics/GraphicsDraw.h"

#include "Input/InputSystem.h"

#include "PlayerComponent.h"
#include "GameTags.h"
#include "Physics/CollisionComponent.h"

#include "GameInputActions.h"

namespace Game
{

	PlayerControllerSubsystem::PlayerControllerSubsystem( Core::Registry & registry, Graphics::Window & windowRef, Input::InputSystem const & inputSystem )
		: Subsystem( registry )
		, m_inputSystem( inputSystem )
	{
		CreateComponents( windowRef );
	}

	void PlayerControllerSubsystem::CreateComponents( Graphics::Window & windowRef )
	{
		auto & renderer = windowRef.GetRenderer();

		Graphics::ShaderComponent const textureEnabledShaderComp = renderer.CreateShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
		Graphics::ShaderComponent const textureMixedShaderComp = renderer.CreateShader( "textureMixing", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
		auto const containerTextureId = renderer.CreateTexture( "container.jpg" );
		auto const awesomefaceTextureId = renderer.CreateTexture( "awesomeface.png" );
		auto const playerTextureId = renderer.CreateTexture( "Player.png" );

		Graphics::TextureComponent const multiTextureComp { { containerTextureId, awesomefaceTextureId } };
		Graphics::TextureComponent const playerTextureComp { { playerTextureId } };

		//Player Entity
		m_PlayerEntity = m_registry.CreateEntity( "Player" );
		
		//Transform
		Graphics::TransformComponent & transformComponent = m_registry.AddComponent<Graphics::TransformComponent>( m_PlayerEntity, Graphics::TransformComponent() );
		transformComponent.Scale = glm::vec3( 2.f, 2.f, 1.f );

		//Mesh - Ideally we should short hand this to "Sprite Component" which automatically makes all the sub components needed
		m_registry.AddComponent<Graphics::MeshComponent>( m_PlayerEntity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
		m_registry.AddComponent<Graphics::TextureComponent>( m_PlayerEntity, playerTextureComp );
		m_registry.AddComponent<Graphics::ShaderComponent>( m_PlayerEntity, textureEnabledShaderComp );

		//Collision
		Physics::CollisionComponent & collisionComponent = m_registry.AddComponent<Physics::CollisionComponent>( m_PlayerEntity );
		collisionComponent.drawDebugLines = true;
		/*Physics::SquareExtents & extents = m_registry.AddComponent<Physics::SquareExtents>( m_PlayerEntity );
		extents.width = 2.f;
		extents.height = 1.21f;
		extents.offset = { 0.f, -0.40f };*/
		Physics::CircleExtents & extents = m_registry.AddComponent<Physics::CircleExtents>( m_PlayerEntity );
		extents.radius = 2.f;

		//Game components
		m_registry.AddComponent<PlayerComponent>( m_PlayerEntity, PlayerComponent() );
		m_registry.AddComponent<PlayerTag>( m_PlayerEntity );
		{
			auto entity = m_registry.CreateEntity( "Player2" );
			m_registry.AddComponent<PlayerComponent>( entity, PlayerComponent() );
			auto & transformComponent = m_registry.AddComponent<Graphics::TransformComponent>( entity, Graphics::TransformComponent() );
			transformComponent.Position = glm::vec3( 7.f, 0.f, 0.f );
			transformComponent.Scale = glm::vec3( 4.f, 4.f, 1.f );

			m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
			m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
			m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureMixedShaderComp );

			auto & collisionComponent = m_registry.AddComponent<Physics::CollisionComponent>( entity );
			collisionComponent.drawDebugLines = true;
			auto & extents = m_registry.AddComponent<Physics::SquareExtents>( entity );
			extents.width = 4.f;
			extents.height = 4.f;
			/*Physics::CircleExtents & extents = m_registry.AddComponent<Physics::CircleExtents>( entity );
			extents.radius = 1.f;*/
		}
	}

	void PlayerControllerSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );

		m_registry.ForView<Graphics::TransformComponent, PlayerComponent, PlayerTag>( [this, &deltaTime] ( Graphics::TransformComponent & transform, PlayerComponent & player)
			{
				if ( m_inputSystem.IsPressesd( PlayerInputActions::MoveLeft ) || m_inputSystem.IsHeld( PlayerInputActions::MoveLeft ) )
				{
					transform.Position.x += player.m_MovementSpeed * static_cast< float >( deltaTime );
				}

				if ( m_inputSystem.IsPressesd( PlayerInputActions::MoveRight ) || m_inputSystem.IsHeld( PlayerInputActions::MoveRight ) )
				{
					transform.Position.x -= player.m_MovementSpeed * static_cast<float>(deltaTime);
				}
			} );

	}

	void PlayerControllerSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );
		m_registry.ForView<Graphics::TransformComponent, PlayerComponent, PlayerTag>( [this] ( Graphics::TransformComponent & transform, PlayerComponent & player )
			{
				glm::dvec3 mousePosition = m_inputSystem.GetMouseWorldPosition();
				transform.Position.x = static_cast< float >( mousePosition.x );
				transform.Position.y = static_cast< float >( mousePosition.y );
			} );
	}

	void PlayerControllerSubsystem::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
	}

}

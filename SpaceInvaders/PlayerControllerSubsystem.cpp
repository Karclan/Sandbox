#include "PlayerControllerSubsystem.h"

#include "Utils/UnusedMacro.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"

#include "PlayerComponent.h"

namespace Game
{

	PlayerControllerSubsystem::PlayerControllerSubsystem( Core::Registry & registry, Graphics::Window & windowRef )
		: Subsystem( registry )
	{
		CreateComponents( windowRef );
	}

	void PlayerControllerSubsystem::CreateComponents( Graphics::Window & windowRef )
	{
		auto & renderer = windowRef.GetRenderer();

		auto textureEnabledShaderId = renderer.CreateShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
		auto containerTextureId = renderer.CreateTexture( "container.jpg" );
		auto awesomefaceTextureId = renderer.CreateTexture( "awesomeface.png" );

		Graphics::TextureComponent multiTextureComp { { containerTextureId, awesomefaceTextureId } };
		Graphics::TextureComponent single { { awesomefaceTextureId } };
		Graphics::ShaderComponent textureEnabledShaderComp { textureEnabledShaderId };

		m_PlayerEntity = m_registry.CreateEntity( "Player" );
		m_registry.AddComponent<PlayerComponent>( m_PlayerEntity, PlayerComponent() );
		auto & transformComponent = m_registry.AddComponent<Graphics::TransformComponent>( m_PlayerEntity, Graphics::TransformComponent() );
		transformComponent.Position = glm::vec3( -7.f, 0.f, 0.f );
		transformComponent.Scale = glm::vec3( 4.f, 4.f, 1.f );

		m_registry.AddComponent<Graphics::MeshComponent>( m_PlayerEntity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
		m_registry.AddComponent<Graphics::TextureComponent>( m_PlayerEntity, multiTextureComp );
		m_registry.AddComponent<Graphics::ShaderComponent>( m_PlayerEntity, textureEnabledShaderComp );


		{
			auto entity = m_registry.CreateEntity( "Player2" );
			m_registry.AddComponent<PlayerComponent>( entity, PlayerComponent() );
			auto & transformComponent = m_registry.AddComponent<Graphics::TransformComponent>( entity, Graphics::TransformComponent() );
			transformComponent.Position = glm::vec3( 7.f, 0.f, 0.f );
			transformComponent.Scale = glm::vec3( 4.f, 4.f, 1.f );

			m_registry.AddComponent<Graphics::MeshComponent>( entity, renderer.GetMesh( Graphics::PrimativeType::Cube ) );
			m_registry.AddComponent<Graphics::TextureComponent>( entity, multiTextureComp );
			m_registry.AddComponent<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
		}
	}

	void PlayerControllerSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void PlayerControllerSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void PlayerControllerSubsystem::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
	}

}

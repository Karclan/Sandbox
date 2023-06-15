#include "GameImpl.h"

#include "ComponentDetails/GameComponentDetails.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/TransformComponent.h"

namespace Game
{

	SpaceInvadersGame::SpaceInvadersGame() 
		: m_PlayerControllerSubsytem( GetRegistry(), GetWindow(), GetInputSystem() )
	{

	}

	void SpaceInvadersGame::Init()
	{
		GameClass::Init();

		RegisterSubsystem( &m_PlayerControllerSubsytem );
	}

	void SpaceInvadersGame::EditorInit()
	{
		auto CameraEntity = m_cameraSystem.CreateOrthographicCamera( "EditorCamera" );
		m_cameraSystem.SetActiveCamera( "EditorCamera" );

		Graphics::CameraComponent & cameraComponent = m_mainRegistry.GetComponent<Graphics::CameraComponent>( CameraEntity );
		cameraComponent.m_farPlane = 100;
		cameraComponent.m_nearPlane = 0.1f;
		cameraComponent.m_orbit = false;
		cameraComponent.m_orthoAspectScale = 10.f;
		Graphics::TransformComponent & transformComponent = m_mainRegistry.GetComponent<Graphics::TransformComponent>( CameraEntity );
		transformComponent.Position.z = -24.f; //For the sake of switching between ortho and perspective
		GameClass::EditorInit();
	}

	void SpaceInvadersGame::DeEditorInit()
	{
		GameClass::DeEditorInit();
	}

	void SpaceInvadersGame::DeInit()
	{
		DeregisterSubsystem( &m_PlayerControllerSubsytem );

		GameClass::DeInit();
	}

	void SpaceInvadersGame::RegisterComponentDetails()
	{
		GameClass::RegisterComponentDetails();
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::PlayerComponentDetails>() ) );

		m_editorInspector.RegisterTagDetailsView( std::move( std::make_unique<Editor::PlayerTagDetails>() ));
	}
}

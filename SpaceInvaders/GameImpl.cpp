#include "GameImpl.h"

#include "ComponentDetails/GameComponentDetails.h"
#include "Graphics/CameraComponent.h"

namespace Game
{

	SpaceInvadersGame::SpaceInvadersGame() 
		: m_PlayerControllerSubsytem( GetRegistry(), GetWindow() )
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
		cameraComponent.m_farPlane = 1;
		cameraComponent.m_nearPlane = -1;
		cameraComponent.m_orbit = false;
		cameraComponent.m_orthoAspectScale = 4.f;
		cameraComponent.m_orthoBottom = -0.9f;
		cameraComponent.m_orthoTop = 0.9f;
		cameraComponent.m_orthoLeft = 1.6f;
		cameraComponent.m_orthoRight = -1.6f;
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
	}
}

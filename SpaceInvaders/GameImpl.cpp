#include "GameImpl.h"

#include "ComponentDetails/GameComponentDetails.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/TransformComponent.h"

namespace Game
{

	SpaceInvadersGame::SpaceInvadersGame()
		: m_PlayerControllerSubsytem( GetRegistry(), GetWindow(), GetInputSystem() )
		, m_GridSubsystem( GetRegistry(), GetWindow() )
	{

	}

	void SpaceInvadersGame::Init()
	{
		GameClass::Init();

		RegisterSubsystem( &m_PlayerControllerSubsytem );
		RegisterSubsystem( &m_GridSubsystem );

		MouseBindings = std::make_shared<GameMouseBindings>();
		KeyboardBindings = std::make_shared<GameKeyboardBindings>();
		m_inputSystem.AddBinding( MouseBindings );
		m_inputSystem.AddBinding( KeyboardBindings );
	}

	void SpaceInvadersGame::EditorInit()
	{
		auto CameraEntity = m_cameraSystem.CreateOrthographicCamera( "EditorCamera" );
		m_cameraSystem.SetActiveCamera( "EditorCamera" );

		Graphics::CameraComponent & cameraComponent = m_mainRegistry.GetComponent<Graphics::CameraComponent>( CameraEntity );
		cameraComponent.m_farPlane = 100.f;
		cameraComponent.m_nearPlane = 0.1f;
		cameraComponent.m_orbit = false;
		cameraComponent.m_orthoAspectScale = 20.f;
		
		Graphics::TransformComponent & transformComponent = m_mainRegistry.GetComponent<Graphics::TransformComponent>( CameraEntity );
		transformComponent.Position.x = 30.f;
		transformComponent.Position.y = 20.f;
		transformComponent.Position.z = -10.f; //For the sake of switching between ortho and perspective

		glm::vec3 rotationEuler = glm::vec3( 0.f, 0.f, 180.f );
		transformComponent.RotationQuat = glm::radians( rotationEuler );
		GameClass::EditorInit();
	}

	void SpaceInvadersGame::DeEditorInit()
	{
		GameClass::DeEditorInit();
	}

	void SpaceInvadersGame::DeInit()
	{
		DeregisterSubsystem( &m_GridSubsystem );
		DeregisterSubsystem( &m_PlayerControllerSubsytem );
		GameClass::DeInit();
	}

	void SpaceInvadersGame::RegisterComponentDetails()
	{
		GameClass::RegisterComponentDetails();
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::PlayerComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::GameGridComponentDetails>() ) );

		m_editorInspector.RegisterTagDetailsView( std::move( std::make_unique<Editor::PlayerTagDetails>() ) );
	}
}
